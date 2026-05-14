jest.mock('./apiClient');

const { fetchWithRetry } = require('./fetchWithRetry');
const { getData } = require('./apiClient');

beforeEach(() => {
  jest.clearAllMocks();
});

describe('fetchWithRetry', () => {
  test('first attempt succeeds', async () => {
    getData.mockResolvedValueOnce({ success: true });

    const result = await fetchWithRetry('https://example.com')

    expect(result).toEqual({ success: true });
    expect(getData).toHaveBeenCalledTimes(1);
    expect(getData).toHaveBeenCalledWith('https://example.com');
  });

  test('second attempt suceeds', async () => {
    getData
      .mockRejectedValueOnce(new Error('server error'))
      .mockResolvedValueOnce({ success: true })

    const result = await fetchWithRetry('https://example.com')

    expect(result).toEqual({ success: true });
    expect(getData).toHaveBeenCalledTimes(2);
  });

  test('all attempts fail', async () => {
    getData
      .mockRejectedValueOnce(new Error('server error'))
      .mockRejectedValueOnce(new Error('server error'))
      .mockRejectedValueOnce(new Error('server error'));

    await expect(fetchWithRetry('https://example.com'))
      .rejects.toThrow('Failed after 3 attempts: server error');

    expect(getData).toHaveBeenCalledTimes(3);
  });

  test('max one attempt and fail', async () => {
    getData.mockRejectedValueOnce(new Error('server error'));

    await expect(fetchWithRetry('https://example.com', 1))
      .rejects.toThrow('Failed after 1 attempts: server error');

    expect(getData).toHaveBeenCalledTimes(1);
  });

});