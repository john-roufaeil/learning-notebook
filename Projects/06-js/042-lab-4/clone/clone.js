var mainDiv = document.getElementById("main");
const original = mainDiv.firstElementChild;

mainDiv.addEventListener("click", function (e) {
    if (e.target !== original) return;

    var clone = original.cloneNode(true);

    var randomR = Math.floor(Math.random() * 256);
    var randomG = Math.floor(Math.random() * 256);
    var randomB = Math.floor(Math.random() * 256);

    clone.style.backgroundColor = `rgb(${randomR}, ${randomG}, ${randomB})`;
    clone.firstElementChild.textContent = `rgb(${randomR}, ${randomG}, ${randomB})`;

    mainDiv.appendChild(clone);
});