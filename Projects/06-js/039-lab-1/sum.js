var n;
do {
    n = prompt("Enter quantity of numbers to sum:");
} while (isNaN(n) || n === null || Number(n) <= 0);

var sum = 0;

for (var i = 0; i < n; i++) {
    var val;
    do {
        val = prompt("Enter a number:");
    } while (isNaN(val) || val === null);
    val = Number(val);
    sum += val;
    if (val == 0 || sum >= 100) break;
}

document.write("The sum is: " + sum);