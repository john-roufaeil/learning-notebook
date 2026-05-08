var params = new URLSearchParams(window.location.search);

var nameInput = params.get("name");
var title = params.get("title");
var address = params.get("address");
var gender = params.get("gender");
var email = params.get("email");
var mobile = params.get("mobile");

var detailsDiv = document.getElementById("details");

var greeting = document.createElement("h2");
greeting.textContent = "Hello " + title + " " + nameInput + "!";
detailsDiv.appendChild(greeting);

var addressLabel = document.createElement("p");
addressLabel.textContent = "Address: " + address;
detailsDiv.appendChild(addressLabel);

var genderLabel = document.createElement("p");
genderLabel.textContent = "Gender: " + gender;
detailsDiv.appendChild(genderLabel);

var emailLabel = document.createElement("p");
emailLabel.textContent = "Email: " + email;
detailsDiv.appendChild(emailLabel);

var mobileLabel = document.createElement("p");
mobileLabel.textContent = "Mobile: " + mobile;
detailsDiv.appendChild(mobileLabel);