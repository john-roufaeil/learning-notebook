var form = document.querySelector("form");
var timeoutId;
var hasInteracted = false;

// Start 30-second timeout
timeoutId = setTimeout(function () {
    if (!hasInteracted) {
        alert("Timeout! You haven't entered any data in 30 seconds.");
        form.reset();
    }
}, 30000);
// bom action

// Reset timeout when user interacts with any form field
var inputs = form.querySelectorAll("input");
inputs.forEach(function (input) {
    input.addEventListener("input", function () {
        hasInteracted = true;
        clearTimeout(timeoutId);
    });
});

function handleSubmit(event) {
    event.preventDefault();
    clearTimeout(timeoutId);

    var name = document.getElementById("name").value;
    var title = document.getElementById("title").value;
    var address = document.getElementById("address").value;
    var gender = document.querySelector('input[name="gender"]:checked').value;
    var email = document.getElementById("email").value;
    var mobile = document.getElementById("mobile").value;

    var params = "?name=" + encodeURIComponent(name) +
        "&title=" + encodeURIComponent(title) +
        "&address=" + encodeURIComponent(address) +
        "&gender=" + encodeURIComponent(gender) +
        "&email=" + encodeURIComponent(email) +
        "&mobile=" + encodeURIComponent(mobile);

    window.location.href = "welcome.html" + params;
}
