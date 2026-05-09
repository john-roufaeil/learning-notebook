var tbody = document.getElementsByTagName("tbody")[0];
var n = 0;
var names = [];
var ages = [];

do {
    n = prompt("Enter number of users:");
} while (!n || isNaN(n) || Number(n) <= 0);


for (var i = 0; i < n; i++) {
    do {
        var nameInput = prompt(`Enter name #${i + 1}:`);
    } while (
        !nameInput || !/^[a-zA-Z\s]+$/.test(nameInput) ||
        nameInput.trim() === "" || nameInput.length < 4 || nameInput.length > 9
    );

    do {
        var ageInput = prompt(`Enter age #${i + 1}:`);
    } while (!ageInput || isNaN(ageInput) || Number(ageInput) > 59 || Number(ageInput) < 11);

    names[i] = nameInput;
    ages[i] = ageInput;

    var trow = document.createElement("tr");
    trow.id = "row";
    trow.innerHTML = `<td>${nameInput}</td><td>${ageInput}</td>`;
    tbody.appendChild(trow);
};


