
var sourceDest = new Array("",""); // [0] is Src, [1] is Dst
var square = "";

/**
 * Highlight a move square
 * 
 * @param cmd
 */
function highlightMove(cmd)
{
  theme="default";
    
  // Clear old highlighting 
  for (i=0; i<2; i++) {
	if (sourceDest[i] != "") {
//	  x = sourceDest[i] % 8;
//	  y = parseInt(sourceDest[i]/8);
//	  if ((y+1+x)%2 == 0) {
//		img = "wsquare.jpg";
//	  }
//	  else {
//		img = "bsquare.jpg";
//	  }
      img = "wsquare.jpg";
	  obj = window.document.getElementById(sourceDest[i]);
	  if (obj) {
		obj.style.backgroundImage = "url(/" + module_path + "/images/" + theme + "/" + img + ")";
      }
	  sourceDest[i] = "";
	}
  }
	
  // If command is empty don't highlight again
  if (cmd == null || cmd == "") {
	return;
  }
		
  // Parse command for source/destination and highlight it 
//  sourceDest[0] = (cmd.charCodeAt(2)-49)*8 + (cmd.charCodeAt(1)-97);
  // cmd is e.g. "Nb1" or "Nb1-c3"
  sourceDest[0] = cmd.substr(1,2); // e.g. "b1"
  if (cmd.length >= 6) {
// 	sourceDest[1] = (cmd.charCodeAt(5)-49)*8+(cmd.charCodeAt(4)-97);
	sourceDest[1] = cmd.substr(5,6); // e.g. "c3"
  }
  else {
	sourceDest[1] = "";
  }
	
  // Hugh having a go at extracting square location
  // e.g. cmd = "Na2"
  square = cmd.substr(1,2);	

  // Set new highlighting
  for (i=0; i<2; i++) {
	if (sourceDest[i] != "") {
	  x = sourceDest[i] % 8;
	  y = parseInt(sourceDest[i]/8);
	  if ((y+1+x)%2==0) {
		img = "whsquare.jpg";  // White square highlighted
	  }
      else {
		img = "bhsquare.jpg";  // Black square highlighted
      }
//	  obj = window.document.getElementById("btd"+sourceDest[i]);
	  obj = window.document.getElementById(square);
	  if (obj) {
//				obj.style.backgroundImage = "url(/"+module_path+"/images/"+theme+"/"+img+")";
				obj.style.backgroundImage = "url(/"+sub_path+"/images/"+theme+"/"+img+")";
	  }
    }
  }
}

/**
 * 
 */
function checkMoveButton()
{
  var cform = window.document.getElementById("vchess-command-form");

  // Move button
  if (cform && window.document.getElementById("edit-moveButton")) {
	if (cform.move.value.length >= 6) {
	  window.document.getElementById("edit-moveButton").disabled=false;		
	}
	else {
	  window.document.getElementById("edit-moveButton").disabled=true;
	}
  }
}

/**
 * Assemble command into commandForm.move and submit move if destination is
 * clicked twice.
 * 
 * @param: part
 *   This might contain something like:
 *     'xb8' = piece on the b8 square (which belongs to the non-moving player)
 *     'Ke1' = King on e1 square
 *     '-b5' = empty b5 square 
 */
function assembleCmd(part)
{
  var cform = window.document.getElementById("vchess-command-form");	
  var cmd = cform.move.value;
  var cmd3onwards = cmd.substring(3);

  // e.g. cmd might contain something like "Pe2-e4"
  if (cmd == part) {
	cform.move.value = "";
  }
  else if (cmd.length == 0 || cmd.length >= 6) {
	if (part.charAt(0) != '-' && part.charAt(0) != 'x') {
	  cform.move.value = part;
	}
//  else if (cmd.length >= 6 && cmd3onwards == part) {
//  if (confirm("Execute move "+cmd+"?")) {
//	onClickMove();
//    }
  } else if (part.charAt(0) == '-' || part.charAt(0) == 'x') {
	  cform.move.value = cmd + part;
  }
  else {
	  cform.move.value = part;
  }

  if (cform.move.value.length >= 6) {
	  onClickMove();
  }
  
  highlightMove(cform.move.value);
  checkMoveButton();
	
  return false;
}

/**
 * Make a move
 * 
 * A move may be a move to a square or a capture, e.g.:
 * - "Pe2-e4"
 * - "Qd1xBg4"
 */
function onClickMove()
{
	var cform = window.document.getElementById("vchess-command-form");
	
	if (cform.move.value != "") {
		var move = cform.move.value;
		var move_type = move[3];
		var to_rank;
		
		// Find out the rank of the square we are going to
		if (move_type == "-") {
		  to_rank = move[5];
		}
		else { // move_type == "x"
		  to_rank = move[6];
		}
		
		// If pawn enters last line ask for promotion
		if (move[0]=='P' && (to_rank=='8' || to_rank=='1')) {
			if (confirm('Promote to Queen? (Press Cancel for other options)'))
				move = move + '=Q';
			else if (confirm('Promote to Rook? (Press Cancel for other options)'))
				move = move + '=R';
			else if (confirm('Promote to Bishop? (Press Cancel for other options)'))
				move = move + '=B';
			else if (confirm('Promote to Knight? (Press Cancel to abort move)'))
				move = move + '=N';
			else
				return;
		}
		cform.cmd.value = move;
		gatherCommandFormData();
		cform.submit();
	}
}

/**
 * Get the user to confirm resignation
 */
function confirm_resign()
{
	var resign = confirm("Are you sure you want to resign?");
	if (resign == true) {
	  alert(Drupal.t("You pressed OK!"));
	}
	else {
	  alert(Drupal.t("You pressed Cancel!"));
	}
}

/**
 * 
 */
function gatherCommandFormData() 
{
	fm = window.document.getElementById("vchess-command-form");
//	if (document.commentForm && document.commentForm.comment)
//		fm.comment.value=document.commentForm.comment.value;
//	if (document.pnotesForm && document.pnotesForm.privnotes)
//		fm.privnotes.value=document.pnotesForm.privnotes.value;
//	else
//		fm.privnotes.disabled=true;
}