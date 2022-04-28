"use strict";
//CSCI2720 Assignment 1 Name: Lai Man Hin, SID: 1155136167
//This typescript file and the compiled file only include "Dynamic Contents" parts' functions only, for all function, please see "dynamic.js"
window.onscroll = function () { scrolling(); };
function scrolling() {
    let bar = document.getElementById('Bar');
    if (bar != null)
        bar.setAttribute("style", 'width: ' + window.scrollY / (document.documentElement.scrollHeight - window.innerHeight) * 100 + '%');
}
;
function changeAlign() {
    let allsection = document.querySelectorAll("h2");
    if (allsection[0].classList.contains('text-start'))
        allsection.forEach(thesection => {
            thesection.classList.replace('text-start', 'text-center');
        });
    else if (allsection[0].classList.contains('text-center'))
        allsection.forEach(thesection => {
            thesection.classList.replace('text-center', 'text-end');
        });
    else if (allsection[0].classList.contains('text-end'))
        allsection.forEach(thesection => {
            thesection.classList.replace('text-end', 'text-start');
        });
    else
        allsection.forEach(thesection => {
            thesection.classList.add('text-center');
        });
}
;
function addHobby() {
    let hobby = prompt('Your new hobby here!');
    let hobbyList = document.getElementById("hobbyList");
    if (hobby != null && hobby != '' && hobbyList != null)
        hobbyList.innerHTML += `<div class="m-2 p-3 bg-ternary">` + hobby + `</div>`;
}
;
function displayScroll() {
    let sc = document.getElementById('scroll');
    if (sc != null) {
        if (sc.style.display === 'none')
            sc.style.display = '';
        else
            sc.style.display = 'none';
    }
}
;