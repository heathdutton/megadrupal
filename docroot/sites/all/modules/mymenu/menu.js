
//function mymenujumpto(x){ if (document.mymenu.jumpmenu.value!='null') { document.location.href = x } } 

var myMenuIsOpen = "no";

function myMenuToogle() {

	if (myMenuIsOpen == "yes") {
		document.getElementById('myMenu-list').style.display = 'none';	
		document.getElementById('myMenu-list').style.height = '0px';	
		document.getElementById('myMenu-list').style.opacity = '0';	
		myMenuIsOpen = "no";
	}
	else {
		document.getElementById('myMenu-list').style.display = 'block';	
		document.getElementById('myMenu-list').style.height = 'auto';	
		document.getElementById('myMenu-list').style.opacity = '1';	
		myMenuIsOpen = "yes";
	}

}
