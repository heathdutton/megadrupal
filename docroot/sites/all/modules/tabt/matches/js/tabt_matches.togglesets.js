/**
 * @file
 * Toggle visibility of setresults.
 */

var show_sets = false;
function togglesets() {
	if (show_sets == true) {
		show_sets = false;
		document.getElementById('settoggle').innerHTML = tabt_matches_togglesets_show;
	} else {
		showsets = true;
		document.getElementById('settoggle').innerHTML = tabt_matches_togglesets_hide;
	}
	var els = getElsByClassName('set');
	for (i in els) {
		if (show_sets == true) {
			els[i].style.display = '';
		} else {
			els[i].style.display = 'none';
		}
	}
}

function getElsByClassName(searchClass,node,tag){
	var classElements = new Array();
	if ( node == null )
		node = document;
	if ( tag == null )
		tag = '*';
	var els = node.getElementsByTagName(tag);
	var elsLen = els.length;
	var pattern = new RegExp("(^|\\s)"+searchClass+"(\\s|$)");
	for (i = 0, j = 0; i < elsLen; i++) {
		if ( pattern.test(els[i].className) ) {
			classElements[j] = els[i];
			j++;
		}
	}
	return classElements;
}