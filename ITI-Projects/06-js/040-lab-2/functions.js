function acceptsTwoParameters(param1, param2) {
    if (arguments.length !== 2) {
        throw new Error('Function requires exactly two parameters.');
    }
    return [param1, param2];
}

function reverseNumbers(...args) {
    // var numbers = [];
    // for (var i = 0; i < arguments.length; i++) {
    //     numbers[i] = arguments[i];
    // }
    return args.reverse();
}

function addNumbersOnly() {
    var sum = 0;
    for (var i = 0; i < arguments.length; i++) {
        if (isNaN(arguments[i])) {
            throw new Error('All parameters must be numbers.');
        }
        sum += Number(arguments[i]);
    }
    return sum;
}

function getDayName(date) {
    var days = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday'];
    return days[date.getDay()];
}

console.log("Accepts two paramters", acceptsTwoParameters(5, 10));
console.log("Reversed numbers", reverseNumbers(1, 2, 3, 4, 5));
console.log("Sum of numbers only", addNumbersOnly(1, 2, 3, 4));
console.log("Day name", getDayName(new Date('01/03/2026')));
// console.log("Accepts two paramters", acceptsTwoParameters(5, 10, 15));
console.log("Sum of numbers only", addNumbersOnly(1, 'a', 2, 3, 4));