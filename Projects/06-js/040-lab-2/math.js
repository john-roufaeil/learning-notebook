do {
    var radius = prompt("Enter the radius of the circle:");
} while (isNaN(radius) || radius <= 0);

var proceed = confirm("The area of the circle is: " + (Math.PI * radius * radius) + ("\nProceed to square?"));

if (proceed) {
    do {
        var square = prompt("Enter the square of the number:");
    } while (isNaN(square) || square <= 0);
    alert("The square root of " + square + " is: " + Math.sqrt(square));
}