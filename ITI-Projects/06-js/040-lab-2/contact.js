var nameInput;
var phone;
var mobile;
var email;

do {
    nameInput = prompt("Enter your name:");
} while (!nameInput || !/^[a-zA-Z\s]+$/.test(nameInput));

do {
    phone = prompt("Enter your phone number:");
} while (isNaN(phone) || phone.length != 8);

do {
    mobile = prompt("Enter your mobile number:");
} while (isNaN(mobile) || !/01[0-2][0-9]{8}/.test(mobile));

do {
    email = prompt("Enter your email address:");
} while (!/^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/.test(email));

document.write("<h1>Your Information:</h2>");
document.write("<p>Name: " + nameInput + "</p>");
document.write("<p>Phone Number: " + phone + "</p>");
document.write("<p>Mobile Number: " + mobile + "</p>");
document.write("<p>Email Address: " + email + "</p>");