/* Part 2 */
console.log('PART 2')

for (var i = 1; i <= 20; i++)
    console.log(i);



/* Part 3 */
console.log('PART 3')

const numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]

for (var i = 0; i < numbers.length; i++) {
    var s = "";
    if (numbers[i] % 3 == 0)
        s += "eple";
    if (numbers[i] % 5 == 0)
        s += "kake";
    if (s.length == 0)
        s += numbers[i];
    console.log(s);
}



/* Part 4 */

document.getElementById('title').innerText = "Hello JavaScript";



/* Part 5 */
function changeDisplay () {
    document.getElementById("magic").style.display = "none";
}

function changeVisibility () {
    var element = document.getElementById("magic");
    element.style.visibility = "hidden";
    element.style.display = "block";
}

function reset () {
    var element = document.getElementById("magic");
    element.style.visibility = "visible";
    element.style.display = "block";
}



/* Part 6 */
const technologies = [
    'HTML5',
    'CSS3',
    'JavaScript',
    'Python',
    'Java',
    'AJAX',
    'JSON',
    'React',
    'Angular',
    'Bootstrap',
    'Node.js'
];

var ul = document.getElementById("tech");

for (var i = 0; i < technologies.length; i++) {
    var element = document.createElement("li");
    element.appendChild(document.createTextNode(technologies[i]));
    ul.appendChild(element);
}
