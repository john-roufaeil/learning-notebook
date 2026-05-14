jest.mock('./paymentService.js')
jest.mock('./emailService.js')

const { placeOrder } = require('./orderService.js');
const { charge } = require('./paymentService.js');
const { sendOrderConfirmation } = require('./emailService.js');

beforeEach(() => {
  jest.clearAllMocks();
});

describe('placeOrder', () => {

  test('a valid order returns an object with both orderId and transactionId', async () => {
    charge.mockResolvedValue({ success: true, transactionId: 'txn_live_abc123' })
    sendOrderConfirmation.mockResolvedValue({ sent: true })

    const result = await placeOrder('123', 'john@email.com', 500)

    expect(charge).toHaveBeenCalledTimes(1);
    expect(charge).toHaveBeenCalledWith('123', 500);
    expect(sendOrderConfirmation).toHaveBeenCalledTimes(1);
    expect(sendOrderConfirmation).toHaveBeenCalledWith('john@email.com', 'txn_live_abc123');

    expect(result).toHaveProperty('orderId')
    expect(result).toHaveProperty('transactionId')
  })

  test('an amount of 0 throws "Invalid amount" — and charge is never called', async () => {
    await expect(placeOrder('123', 'john@email.com', 0))
      .rejects.toThrow('Invalid amount');
    expect(charge).not.toHaveBeenCalled();
  })

  test('function throws "Payment failed" when charge doesnt resolve — and sendOrderConfirmation is never called ', async () => {
    charge.mockResolvedValue({ success: false })

    await expect(placeOrder('123', 'john@email.com', 500))
      .rejects.toThrow('Payment failed');
    expect(sendOrderConfirmation).not.toHaveBeenCalled();
  })
})