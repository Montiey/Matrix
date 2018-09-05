var charHeight = 16;
var charWidth = 12;
var charSpacing = 0;

var speedMin = 5;	//px per refresh
var speedMax = 20;

var cols = Math.floor(($("#apoc").width() - charWidth) / (charWidth + charSpacing)) + 1;

var length = 16;

addElements();

setInterval(updateElements, 30);

function addElements(){	//Add DOM elements once
	for(var i = 0; i < cols; i++){
		console.log("new element");
		var container = document.createElement("div");
		container.classList.add("char");
		container.innerHTML = randChar();

		container.style.height = charHeight + "px";
		container.style.width = charWidth + "px";
		container.style.lineHeight = charHeight + "px";
		container.style.fontSize = charHeight + "px";
		container.style.textAlign = "center";

		container.setAttribute("data-y", Math.random() * $("#apoc").height() + "px");
		container.style.top = container.getAttribute("data-y");	//set both immediatly so the positions don't jump when loading
		
		container.style.left = i * charWidth + i * charSpacing + "px";
		
		container.setAttribute("data-speed", Math.random() * (speedMax - speedMin) + speedMin);

		document.getElementById("apoc").appendChild(container);
	}
}

function updateElements(){	//Run through chars, increment stuff, wrap stuff, etc.
	$.each(document.getElementsByClassName("char"), function(index, value){
		var tempHeight = parseFloat(value.getAttribute("data-y") || 0);
		tempHeight += parseFloat(value.getAttribute("data-speed"));
		if(tempHeight >= $("#apoc").height()){
			tempHeight = -charHeight;
		}
		value.setAttribute("data-y", tempHeight);
		value.style.top = tempHeight + "px";
		
//		console.log("float: " + tempHeight + " int: " + value.style.top);
	});
}

function randChar(){
	return Math.floor(Math.random() * 10);
}