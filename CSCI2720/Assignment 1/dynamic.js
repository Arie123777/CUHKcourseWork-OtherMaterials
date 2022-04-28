//CSCI2720 Assignment 1 Name: Lai Man Hin, SID: 1155136167
let id;
window.onscroll =function() {scrolling()};
window.onload = function() {pageLoad()};
function pageLoad() {
    fetch('comments.json').then(read => read.json()).then(readfile => {
        readfile.comments.forEach(theComment =>{
            document.getElementById("comments").innerHTML += 
            `<div id="${theComment.id}" class="d-flex">
                <div class="flex-shrink-0">
                    <svg width="100" height="100">
                        <circle  cx="50" cy="50" r="40" fill="${theComment.color}"></circle>
                    </svg>
                </div>
                <div class="flex-grow-1">
                    <h5>${theComment.email}</h5>
                    <p>${theComment.content}</p>
                </div>
            </div>`;
            })
            id = (readfile.comments.length+10000).toString();
        })
};
function scrolling() {
    document.getElementById('Bar').style = 'width: ' + window.scrollY/(document.documentElement.scrollHeight-window.innerHeight)*100 + '%';
};

function displaySpecial(){
    let sp=document.getElementById('special');
    if (sp.style.display === 'none')
        sp.style.display ='block';
    else
        sp.style.display='none';
};
function changeAlign(){
    let allsection=document.querySelectorAll("h2");
    if (allsection[0].classList.contains('text-start'))
        allsection.forEach(thesection => {
            thesection.classList.replace('text-start', 'text-center')
        });
    else if (allsection[0].classList.contains('text-center'))
        allsection.forEach(thesection => {
            thesection.classList.replace('text-center', 'text-end')
        });
    else if (allsection[0].classList.contains('text-end'))
        allsection.forEach(thesection => {
            thesection.classList.replace('text-end', 'text-start')
        });
    else
        allsection.forEach(thesection => {
            thesection.classList.add('text-center')
        });
};
function addHobby(){
    let hobby = prompt('Your new hobby here!');
    if (hobby != null && hobby != '')
        document.getElementById("hobbyList").innerHTML += `<div class="m-2 p-3 bg-ternary">`+ hobby + `</div>`;
};

function displayScroll() {
    let sc = document.getElementById('scroll');
    if (sc != null) {
        if (sc.style.display === 'none')
            sc.style.display = '';
        else
            sc.style.display = 'none';
    }
}

function processForm(){
    let email=document.getElementById('my-address');
    let content = document.getElementById('my-message');
    let color;
    if (!email.checkValidity())
        email.classList.add('is-invalid');
    else
        email.classList.remove('is-invalid')
    if (!content.checkValidity())
        content.classList.add('is-invalid');
    else
        content.classList.remove('is-invalid');
    if (!(email.checkValidity() && content.checkValidity()))
        return;
    color = document.querySelector('input[name="my-color"]:checked').id.slice(9);
    let output = {
        'id': 'c'+id,
        'email': email.value,
        'content': content.value,
        'color': color
    }
    let output_promise = fetch('comments.json').then(read => read.json()).then(readfile => {
        readfile.comments.push(output);
        let output_promise = JSON.stringify(readfile);
        return output_promise;
        });
    Promise.resolve(output_promise).then(output_file => {
        fetch('comments.json', {
        method : 'PUT',
        body : output_file})});
    document.getElementById("comments").innerHTML += 
            `<div id="${'c'+id}" class="d-flex">
                <div class="flex-shrink-0">
                    <svg width="100" height="100">
                        <circle  cx="50" cy="50" r="40" fill="${color}"></circle>
                    </svg>
                </div>
                <div class="flex-grow-1">
                    <h5>${email.value}</h5>
                    <p>${content.value}</p>
                </div>
            </div>`;
    document.getElementById('my-address').value='';
    document.getElementById('my-message').value='';
    id=id+1;
}

