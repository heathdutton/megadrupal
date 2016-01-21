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
      emailInput.focus(function() {
        var currentVal = emailInput.val().replace(/^\s\s*/, '').replace(/\s\s*$/, '');
        if (currentVal.length > 1) {
          autoShow();
        }
      });
      //bind blur event
      emailInput.blur(autoHide);
      
      emailListDiv.addClass("autoDiv");
      //bind the events:mouseover、mouseout for the div
      for (var i = 0; i < emailList.length; i++) {
        $("#mail-autocomplete").append("<div id='email-" + (i + 1).toString() + "' class='item'>" + emailList[i] + "</div>");
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
      emailInput.keyup(function(event) {
        var myEvent = event || window.event;
        //get key value
        var keyCode = myEvent.keyCode; 
        //press down key
        if (keyCode == 40) {
          emailList_oldIndex = emailList_newIndex;
          emailList_newIndex++;
          setStyleForChange(emailList_counts, emailList_oldIndex, emailList_newIndex)
          //set value for input
          setValue(emailList_newIndex, emailInput);
        }
        //press up key
        if (keyCode == 38) {
          emailList_oldIndex = emailList_newIndex;
          emailList_newIndex--;
          setStyleForChange(emailList_counts, emailList_oldIndex, emailList_newIndex)
          //set value for input
          setValue(emailList_newIndex, emailInput);
        }
        //press esc key
        if (keyCode == 27) {
          autoHide();
        }
        //press a-z|A-Z|0-9 
        var changedText = (keyCode >= 65 && keyCode <= 90) || (keyCode >= 97 && keyCode <= 122) || (keyCode >= 48 && keyCode <= 56);
        var currentVal = emailInput.val().replace(/^\s\s*/, '').replace(/\s\s*$/, '');
        if (changedText) {
          //check @
          if (currentVal.indexOf("@") > -1) {
            autoHide();
          } else if (document.getElementById("mail-autocomplete").style.visibility != "visible" && currentVal.length > 1) {
            autoShow();
          }
          for (var i = 1; i <= emailList.length; i++) {
            $("div #email-" + i).text("").text(currentVal + emailList[i - 1]);
          }
        }
        //back space or del
        if (keyCode == 8 || keyCode == 46) {
          //remove domain at once
          var domain_index = currentVal.indexOf("@");
          if (domain_index > -1) {
            currentVal = currentVal.substr(0,domain_index);
            emailInput.val(currentVal);
          } else if (document.getElementById("mail-autocomplete").style.visibility != "visible") {
            autoShow();
          }
          for (var i = 1; i <= emailList.length; i++) {
            $("div #email-" + i).text("").text(currentVal + emailList[i - 1]);
          }
        }
      });
      
    });
  },
  //detach: function (context) {}
};

//mousein
function setStyle(obj, emailList_oldIndex, emailList_newIndex) {
  emailList_oldIndex = emailList_newIndex;
  //emailList_newIndex = $(obj).attr("id");
  emailList_newIndex_object = $(obj);
  $(obj).css("background-color", "#E8F4FC");
  $(obj).css("font-weight", "bold");
  //$("div #email-" + emailList_oldIndex).css("background-color", "white");
  //$("div #email-" + emailList_oldIndex).css("font-weight", "bold");
  //$("div #email-" + emailList_oldIndex).css({"font-weight":"normal"});
  setValue(emailList_newIndex_object, $("#edit-mail"));
}

//monuseout
function cancelStyle(obj) {
  $(obj).css("background-color", "white");
  $(obj).css("font-weight", "normal");
}

//autoshow
function autoShow() {
  var mailAddressList = document.getElementById("mail-autocomplete");
  mailAddressList.style.visibility = "visible";
  var position = $("#edit-mail").position();
  var height = $("#edit-mail").height();
  $("#mail-autocomplete").css({"left":position.left, "top":position.top+height+10});
}

//autohide
function autoHide() {
  var mailAddressList = document.getElementById("mail-autocomplete");
  mailAddressList.style.visibility = "hidden";
}

//setvalue
function setValue(emailList_newIndex_object, emailInput) {
  //var addr = $("div #email-" + newIndex).text().replace(/^\s\s*/, '').replace(/\s\s*$/, '');console.log(newIndex);
  var addr = emailList_newIndex_object.text().replace(/^\s\s*/, '').replace(/\s\s*$/, '');
  emailInput.val("");
  emailInput.val(addr);
}

//updown
function setStyleForChange(counts, emailList_oldIndex, emailList_newIndex) {
  //handle newIndex
  emailList_newIndex = emailList_newIndex > counts ? 1 : emailList_newIndex;
  emailList_newIndex = emailList_newIndex < 1 ? counts : emailList_newIndex;
  $("div #email-" + emailList_newIndex).css("background-color", "#E8F4FC");
  $("div #email-" + emailList_newIndex).css("font-weight", "bold");
  $("div #email-" + emailList_oldIndex).css("background-color", "white");
  $("div #email-" + emailList_oldIndex).css("font-weight", "normal");
}

})(jQuery);
