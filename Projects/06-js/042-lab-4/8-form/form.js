var nameInput = document.querySelector("#name input");
var nameSpan = document.querySelector("#name span");
var ageInput = document.querySelector("#age input");
var ageSpan = document.querySelector("#age span");
var emailInput = document.querySelector("#email input");
var emailSpan = document.querySelector("#email span");
var tbody = document.getElementById("table-body");

function submitContact(e) {
    e.preventDefault();

    if (nameSpan.style.display === "block" ||
        ageSpan.style.display === "block" ||
        emailSpan.style.display === "block") {
        alert("Please fix the errors before submitting the form.");
        return;
    }
    var newRow = document.createElement("tr");

    var nameCell = document.createElement("td");
    nameCell.textContent = nameInput.value;
    newRow.appendChild(nameCell);
    var ageCell = document.createElement("td");
    ageCell.textContent = ageInput.value;
    newRow.appendChild(ageCell);
    var emailCell = document.createElement("td");
    emailCell.textContent = emailInput.value;
    newRow.appendChild(emailCell);
    tbody.appendChild(newRow);

    nameInput.value = "";
    ageInput.value = "";
    emailInput.value = "";
}

function nameInputChange(e) {
    if (!e.target.value) {
        nameSpan.textContent = "name is required";
        nameSpan.style.display = "block";
    } else if (/^[a-zA-Z]+$/.test(e.target.value) === false) {
        nameSpan.textContent = "name should be characters";
        nameSpan.style.display = "block";
    } else {
        nameSpan.style.display = "none";
    }
}

function ageInputChange(e) {
    if (!e.target.value) {
        ageSpan.textContent = "age is required";
        ageSpan.style.display = "block";
    } else if (isNaN(e.target.value) || e.target.value < 1 || e.target.value > 120) {
        ageSpan.textContent = "age should be a number between 1 and 120";
        ageSpan.style.display = "block";
    } else {
        ageSpan.style.display = "none";
    }
}

function emailInputChange(e) {
    if (!e.target.value) {
        emailSpan.textContent = "email is required";
        emailSpan.style.display = "block";
    } else if (!/^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/.test(e.target.value)) {
        emailSpan.textContent = "email should be a valid email address";
        emailSpan.style.display = "block";
    } else {
        emailSpan.style.display = "none";
    }
}