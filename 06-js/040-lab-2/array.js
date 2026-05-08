var nums = [];

for (var i = 0; i < 3; i++) {
    do {
        var input = prompt("Enter number " + (i + 1) + ":");
    } while (isNaN(input) || input === null || (i == 2 && Number(input) == 0));
    nums[i] = input;
}

var sum = Number(nums[0]) + Number(nums[1]) + Number(nums[2]);
var multiplication = Number(nums[0]) * Number(nums[1]) * Number(nums[2]);
var division = Number(nums[0]) / Number(nums[1]) / Number(nums[2]);

document.write("<p><span style=\"color: red\">Sum of the 3 values: </span>" + nums.join("+") + " = " + sum + "</p>");
document.write("<p><span style=\"color: red\">Multiplication of the 3 values: </span>" + nums.join("*") + " = " + multiplication + "</p>");
document.write("<p><span style=\"color: red\">Division of the 3 values: </span>" + nums.join("/") + " = " + division + "</p>");