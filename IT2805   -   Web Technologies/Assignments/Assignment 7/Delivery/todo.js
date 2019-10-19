var tasks = [];

var checked = 0;

var inputField = document.getElementById("inputField");
var button = document.getElementById("addInput");
var output = document.getElementById("output");
var ul = document.getElementById("list");



function addTask() {
	if (inputField.value.length > 0) {
		tasks.unshift(inputField.value);
    	var li = document.createElement("li");
		li.appendChild(document.createTextNode(tasks[0]));
		ul.prepend(li);
		inputField.value = "";
		inputField.focus();
	}
}

function updateOutput() {
	output.innerText = "Completed " + checked + " out of " + tasks.length + " tasks!"
}



button.addEventListener("click", () => {
	addTask();
	updateOutput();
});

inputField.addEventListener("keyup", function(event) {
	if (event.which == 13 || event.keyCode == 13) {
		addTask();
		updateOutput();
	}
});

ul.addEventListener("click", function(event) {
	if (event.target.tagName === "LI") {
		event.target.classList.toggle("checked");
		if (event.target.classList.contains("checked"))
			checked++;
		else
			checked--;
	}
	updateOutput();
}, false);



