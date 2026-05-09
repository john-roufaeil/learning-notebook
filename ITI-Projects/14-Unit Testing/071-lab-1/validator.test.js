const { isValidPassword } = require('./validator');

describe('isValidPassword', () => {

  test('returns valid for a strong password', () => {
    const result = isValidPassword('StrongPassword_123');
    expect(result).toEqual({ valid: true, reason: '' });
  });

  test('returns invalid for non-string password', () => {
    const expectedResponse = { valid: false, reason: 'Password must be a string' };
    expect(isValidPassword(1234567890)).toEqual(expectedResponse);
    expect(isValidPassword(null)).toEqual(expectedResponse);
    expect(isValidPassword(undefined)).toEqual(expectedResponse);
    expect(isValidPassword(['Valid_PW123'])).toEqual(expectedResponse);
  });

  test('returns invalid for a short password', () => {
    const result = isValidPassword('Short_1');
    expect(result).toEqual({ valid: false, reason: 'Too short (min 8 characters)' });
  });

  test('returns invalid for non-uppercase password', () => {
    const result = isValidPassword('lower_case123');
    expect(result).toEqual({ valid: false, reason: 'Must contain an uppercase letter' });
  });

  test('returns invalid for letters only password', () => {
    const result = isValidPassword('LettersOnlyPassword');
    expect(result).toEqual({ valid: false, reason: 'Must contain a number' });
  });

  test('returns valid for exactly 8 characters password', () => {
    const result = isValidPassword('PWExact8');
    expect(result).toEqual({ valid: true, reason: '' });
  });

  test('returns invalid for empty string password', () => {
    const result = isValidPassword('');
    expect(result).toEqual({ valid: false, reason: 'Too short (min 8 characters)' });
  });

  test('returns valid for extremely long password', () => {
    const result = isValidPassword('1A' + 'a'.repeat(10000));
    expect(result).toEqual({ valid: true, reason: '' });
  });
});
