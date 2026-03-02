document.addEventListener("DOMContentLoaded", () => {
    const button = document.getElementById("toggle-image-btn");
    const image = document.getElementById("server-image");

    button.addEventListener("click", () => {
        if (image.style.display === "none" || image.style.display === "") {
            image.style.display = "block";
            button.textContent = "Hide Image";
        } else {
            image.style.display = "none";
            button.textContent = "Show Image";
        }
    });
});