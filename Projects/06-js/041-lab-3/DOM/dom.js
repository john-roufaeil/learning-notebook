var headerDiv = document.getElementById("header");
headerDiv.style.height = "33%";

var originalImg = document.getElementsByTagName("img")[0];
originalImg.style.float = "right";

var cloneImg = originalImg.cloneNode(true);
cloneImg.style.float = "left";

var newDiv = document.createElement("div");
newDiv.appendChild(cloneImg);
// newDiv.style.height = "33%";

var nav = document.getElementById("nav");
nav.style.listStyleType = "circle";
nav.style.listStylePosition = "inside";

var navigation = document.getElementById("navigation");
// navigation.style.display = "block";
nav.style.height = "33%";

navigation.appendChild(newDiv);