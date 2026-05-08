var nameInput;
var birthYearInput;

do {
    nameInput = prompt("Enter your name:");
} while (nameInput === null || nameInput.trim() === "" | !/^[A-Za-z\s]+$/.test(nameInput));

do {
    birthYearInput = prompt("Enter your birth year:");
} while (isNaN(birthYearInput) || birthYearInput === null || Number(birthYearInput) >= 2010 || Number(birthYearInput) <= 1900);

document.write("<p>Name: " + nameInput + "</p>");
document.write("<p>Birth Year: " + birthYearInput + "</p>");
document.write("<p>Age: " + (2026 - birthYearInput) + "</p>");