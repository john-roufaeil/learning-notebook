var inputString = prompt("Enter your string:");
var checkCase = confirm("Do you want to check case sensitivity?");

if (!checkCase)
    inputString = inputString.toLowerCase();
var reversedString = inputString.split('').reverse().join('');

if (inputString === reversedString) {
    document.write("<p>The string \"" + inputString + "\" is a <span style=\"color:green;\">palindrome</span>.</p>");
} else {
    document.write("<p>The string \"" + inputString + "\" is <span style=\"color:red;\">not a palindrome</span>.</p>");
}