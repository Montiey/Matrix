var charHeight = 16;
var charWidth = 12;
var charSpacing = 0;

var speedMin = 5;	//px per refresh
var speedMax = 20;

var cols = Math.floor(($("#apoc").width() - charWidth) / (charWidth + charSpacing)) + 1;

var length = 16;

var charString = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~[]\\{}|<>?/"

addElements();

setInterval(updateElements, 30);

function addElements(){	//Add DOM elements once
	for(var i = 0; i < cols; i++){
//		console.log("new element");
		var container = document.createElement("div");
		container.classList.add("char");
		container.style.lineHeight = charHeight + "px";
		container.style.fontSize = charHeight + "px";
		container.style.textAlign = "center";
		
		container.style.top = container.getAttribute("data-y");	//set both immediatly so the positions don't jump when loading
		container.style.left = i * charWidth + i * charSpacing + "px";
		
		container.setAttribute("data-speed", Math.random() * (speedMax - speedMin) + speedMin);
		
		container.style.height = charHeight + "px";
		container.style.width = charWidth + "px";
		
		container.setAttribute("data-y", 0);
		
		container.style.backgroundColor = "rgb(255, 255, 255)";	//color incrementation needs this initialization
		
		document.getElementById("apoc").appendChild(container);
	}
}

function updateElements(){	//Run through chars, increment stuff, wrap stuff, etc.
	$.each(document.getElementsByClassName("char"), function(index, value){
		var tempHeight = parseFloat(value.getAttribute("data-y"));
		tempHeight += parseFloat(value.getAttribute("data-speed"));
		if(tempHeight >= $("#apoc").height()){
			
			var str = value.style.backgroundColor;
			var r parseInt()
			var g
			var b
			
			tempHeight -= parseFloat(value.style.height) + parseFloat($("#apoc").height());	//maintain minute offsets to acheive entropy
		}
		value.setAttribute("data-y", tempHeight);
		value.style.top = tempHeight + "px";
	});
}

function randChar(){
	return charString.charAt(Math.random() * charString.length);
}