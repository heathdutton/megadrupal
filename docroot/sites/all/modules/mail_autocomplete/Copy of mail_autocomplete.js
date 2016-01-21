/**
 * @file
 * Provides JavaScript for Mail Autocomplete.
 */

(function ($) {

Drupal.behaviors.mailAutocomplete = {
  attach: function (context) {
    var emailList = ["@qq.com", "@163.com", "@126.com", "@yahoo.com.cn", "@sina.cn", "@hotmail.com", "@sohu.com", "@189.cn"];
    var emailList_newIndex = 0;
    var emailList_oldIndex = 0;
    var emailList_counts = emailList.length;
    
    $(document).ready(function () {
      var emailInput = $("#edit-mail");
      var emailListDiv = $("#mail-autocomplete");
      //bind focus event
      emailInput.focus(autoShow);
      //bind blur event
      emailInput.blur(autoHide);
      
      emailListDiv.addClass("autoDiv");
      //bind the events:mouseover、mouseout for the div
      for (var i = 0; i < emailList.length; i++) {
        $("#mail-autocomplete").append("<div id='email-" + (i + 1).toString() + "' class='item'>" + emailList[i] + "</div>");
        //$("#mail-autocomplete").append("<div id='" + (i + 1).toString() + "' onmouseover='setStyle(this)' onmouseout='cancelStyle(this)' >" + emailList[i] + "</div>");
      }
      
      //handle mouse events
      $('#mail-autocomplete div.item').each(function(index) {
        $(this).mouseover(function(){
          setStyle(this, emailList_oldIndex, emailList_newIndex);
        });
        $(this).mouseout(function(){
          cancelStyle(this);
        });
      });
      
      //handle key's events
      emailInput.keyup(function (event) {
        var myEvent = event || window.event;
        //get key value
        var keyCode = myEvent.keyCode; 
        //press down key
        if (keyCode == 40) {
          oldIndex = newIndex;
          newIndex++;
          setStyleForChange();
          //set value for input
          setValue(newIndex, emailInput);
        }
        //press up key
        if (keyCode == 38) {
          oldIndex = newIndex;
          newIndex--;
          setStyleForChange();
          //set value for input
          setValue(newIndex, emailInput);
        }
        //press enter key
        if (keyCode == 13) {
          //set value for input
          setValue(newIndex, emailInput);
          //set div hidden
          autoHide();
        }
        //press esc key
        if (keyCode == 27) {
          autoHide();
        }
        //press a-z|A-Z|0-9 
        var changedText = (keyCode >= 65 && keyCode <= 90) || (keyCode >= 97 && keyCode <= 122) || (keyCode >= 48 && keyCode <= 56);
        if (changedText) {
          var currentVal = emailInput.val().replace(/^\s\s*/, '').replace(/\s\s*$/, '');
          //check @
          if (currentVal.indexOf("@") > -1) {
            emailInput.val("");
            for (var i = 1; i <= 6; i++) {
              $("div #" + i).text("").text(emailList[i - 1]);
            }
             return;
          }
          for (var i = 1; i <= 6; i++) {
            $("div #" + i).text("").text(currentVal + emailList[i - 1]);
          }
        }
        //space or del
        if (keyCode == 8 || keyCode == 46) {
          emailInput.val("");
          for (var i = 1; i <= 6; i++) {
            $("div #" + i).text("").text(emailList[i - 1]);
          }
        }
      });
    });
    
    
    
  },
  //detach: function (context) {}
};

//mousein
function setStyle(obj, oldIndex, newIndex) {
  oldIndex = newIndex;
  newIndex = $(obj).attr("id");
  $(obj).css("background-color", "#E8F4FC");
  //$(obj).css({"background-color":"#E8F4FC"});
  $("div #" + oldIndex).css("background-color", "white");
  setValue(newIndex, $("#edit-mail"));
}

//monuseout
function cancelStyle(obj) {
  $(obj).css("background-color", "white");
}

//autoshow
function autoShow() {
  /*
    var obj = document.getElementById("edit-mail");
    var mailAddressList = document.getElementById("mail-autocomplete");
    var x = 0, y = 0, o = obj; h = obj.offsetHeight;
    while (o != null) {
        x += o.offsetLeft;
        y += o.offsetTop;
        o = o.offsetParent;
    }
    mailAddressList.style.left = x + 'px';
    mailAddressList.style.top = y + h + 'px';
    mailAddressList.style.visibility = "visible";*/
  var mailAddressList = document.getElementById("mail-autocomplete");
  mailAddressList.style.visibility = "visible";
  var position = $("#edit-mail").position();
  var height = $("#edit-mail").height();
  $("#mail-autocomplete").css({"left":position.left, "top":position.top+height+10});
  //$("#mail-autocomplete").show();
}

//autohide
function autoHide() {
  var mailAddressList = document.getElementById("mail-autocomplete");
  mailAddressList.style.visibility = "hidden";
}

//setvalue
function setValue(newIndex, emailInput) {
  var addr = $("div #" + newIndex).text().replace(/^\s\s*/, '').replace(/\s\s*$/, '');
  emailInput.val("");
  emailInput.val(addr);
}

//updown
function setStyleForChange() {
    //handle newIndex
    newIndex = newIndex > counts ? 1 : newIndex;
    newIndex = newIndex < 1 ? counts : newIndex;
    $("div #" + newIndex).css("background-color", "red");
    $("div #" + oldIndex).css("background-color", "white");
}

})(jQuery);
