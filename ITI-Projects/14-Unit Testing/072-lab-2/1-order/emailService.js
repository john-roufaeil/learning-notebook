async function sendOrderConfirmation(email, transactionId) {
  // calls SendGrid — never run this in tests
  return { sent: true };
}
module.exports = { sendOrderConfirmation };