for (var i = 1; i <= 6; i++) {
    do {
        var header = prompt("Enter header text for H" + i + ":");
    } while (header === null || header.trim() === "");
    document.write("<h" + i + ">" + header + "</h" + i + ">");
}