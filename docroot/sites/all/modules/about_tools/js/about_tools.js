;
var widg;

(function($) {
  
  // make the email link work
  $('.link-about-tools-email a').click(function() {
    $.ajax({
      url : "/about_tools/mailman/new",
      cache : false,
      dataType : 'json',
      error : function(XMLHttpRequest, textStatus, errorThrown) {
        alert('network error');
      },
      success : function(data, textStatus, XMLHttpRequest) {
        if (data.error) {
          alert ('an error has occured, please try again later').
          $(this).dialog('close');
          return;
        }
        var div = $('<div>');
        var name = $('<input type="text" value="Your Name" class="email-name-'+data.ticket+'" />');
        var email = $('<input type="text" value="Your Email" class="email-email-'+data.ticket+'" />');
        var subj = $('<input type="text" value="Subject" class="email-subj-'+data.ticket+'" />');
        var mssg = $('<textarea rows="6" cols="50" class="email-mssg-'+data.ticket+'"></textarea>');
        $(name).css({'color':'#ccc', 'display':'block'}).appendTo(div);
        $(email).css({'color':'#ccc', 'display':'block'}).appendTo(div);
        $(subj).css({'color':'#ccc', 'display':'block'}).appendTo(div);
        $(mssg).appendTo(div);
        $(div).dialog({
          buttons : {
            'Cancel' : function() { $(this).dialog('close'); }, 
            'Send'   : function() {
              $.ajax({
                url : "/about_tools/mailman/send/" + data.ticket,
                data : {
                  n : $('.email-name-'+data.ticket).val(),
                  e : $('.email-email-'+data.ticket).val(),
                  s : $('.email-subj-'+data.ticket).val(),
                  m : $('.email-mssg-'+data.ticket).val()
                },
                type : 'POST',
                cache : false,
                dataType : 'json',
                context : this,
                error : function(XMLHttpRequest1, textStatus1, errorThrown1) {
                  alert('network error');
                },
                success : function(data1, textStatus1, XMLHttpRequest1) {
                  if (data1.error) {
                    alert ('an error has occured, please try again later').
                    $(this).dialog('destroy');
                    return;
                  }
                  $(this).dialog('option', {
                    buttons : {},
                    modal : false // this doesn't work!
                  });
                  // ...so we have to remove the modal ourselves...
                  $('.ui-widget-overlay').remove();
                  $('.ui-dialog .ui-dialog-content').html('<span style="text-align:center; width: 100%;">Mail Sent.</span>');
                  widg = $(this).dialog('widget');
                  $(widg).fadeOut(2500);
                }
              });
            }
          },
          closeOnEscape : true,
          modal : true,
          title : 'Send an email',
          width : '460px',
          close : function() { $(this).dialog('destroy'); }
        });
        $('.email-name').focus(function() {
          if ($(this).val() == 'Your Name') { $(this).val('').css('color', '#111'); }
        }).blur(function() {
          if ($(this).val() == '') { $(this).val('Your Name').css('color', '#ccc'); }
        });
        $('.email-email').focus(function() {
          if ($(this).val() == 'Your Email') { $(this).val('').css('color', '#111'); }
        }).blur(function() {
          if ($(this).val() == '') { $(this).val('Your Email').css('color', '#ccc'); }
        });
        $('.email-subj').focus(function() {
          if ($(this).val() == 'Subject') { $(this).val('').css('color', '#111'); }
        }).blur(function() {
          if ($(this).val() == '') { $(this).val('Subject').css('color', '#ccc'); }
        });
        return false;
      }
    });
    
    // diffuse the <a href> click
    return false;
  });
  
})(jQuery);