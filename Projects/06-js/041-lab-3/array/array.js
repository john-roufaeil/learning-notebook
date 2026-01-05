var arr = [];

for (var i = 0; i < 5; i++) {
    do {
        var value = prompt("Enter element " + (i + 1) + ":");
    } while (!value || isNaN(value));
    arr[i] = value;
}

document.write("<h1>Sorting</h1>");
document.write("You entered: " + arr.join(", ") + "<br><br>");

var descending = arr.sort(function (a, b) { return b - a; });
document.write("Descending order: " + descending.join(", ") + "<br><br>");

// var ascending = arr.sort(function (a, b) { return a - b; });
var ascending = descending.reverse();
document.write("Ascending order: " + ascending.join(", "));
