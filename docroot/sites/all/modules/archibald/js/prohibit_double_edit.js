var archibald_prohibit_double_edit_interval=0;

/**
 * ping for updating editor last seen bit
 * will called by interval every 3 minutes
 **/
function archibald_prohibit_double_edit_ping()
{
    jQuery.get(Drupal.settings.archibald_prohibit_double_edit.url_ping, function() { } );
}

(function ($) {

Drupal.behaviors.archibald_prohibit_double_edit = {
  attach: function (context, settings) {
    archibald_prohibit_double_edit(settings);

    if (Drupal.settings.archibald_prohibit_double_edit.form_blur_message==true) {
        archibald_form_blur_message();
    }
  }
};

/**
* check if resource is currently edited by other user
* by using Drupal.settings
* and display message dialog in case
* or start interval ping to update editor last seen bit in db
*/
function archibald_prohibit_double_edit(settings) {
    if (settings.archibald_prohibit_double_edit.free!=1) {
        var last_seen = new Date(settings.archibald_prohibit_double_edit.last_seen*1000);

        archibald_prohibit_double_edit_dialog( Drupal.t('User @username is editing this description since @time',
              { '@username': settings.archibald_prohibit_double_edit.user,
                '@time':     last_seen.toLocaleString()
              } )
             );

    } else {
        archibald_prohibit_double_edit_init();
    }
}

/**
 * start ping interval
 * to update editor last seen bit in db
 **/
function archibald_prohibit_double_edit_init() {
    archibald_prohibit_double_edit_ping();

    if (archibald_prohibit_double_edit_interval==undefined) {
        // work arround for for ajax initer
        return false;
    }

    // ping alle 3 Minutes that you still edit this node
    archibald_prohibit_double_edit_interval = window.setInterval("archibald_prohibit_double_edit_ping()", (1000*60*3));

    $('.archibald_form_submit_top').click(archibald_prohibit_double_edit_checkout); // form save button
    $('.archibald_form_submit').click(archibald_prohibit_double_edit_checkout); // form save button
    $('.tabs .primary li span' ).click(archibald_prohibit_double_edit_checkout); // primary tabs like "view", "translate", "delete" ,"revisions"

    $(window).unload(function() {
        archibald_prohibit_double_edit_checkout();
    });
}

/**
 * reset editor last seen bit
 * call this wenn leveing editing mode
 **/
function archibald_prohibit_double_edit_checkout()
{
    $.get(Drupal.settings.archibald_prohibit_double_edit.url_checkout, function() { } );

    window.clearInterval(archibald_prohibit_double_edit_interval);
    archibald_prohibit_double_edit_interval = 0;
}

/**
 * dsiplay a info dialog
 * with history.back() on closing
 *
 * @param message strng
 */
function archibald_prohibit_double_edit_dialog( message )
{
    var html  = '<div id="double_editing_info" title="' + Drupal.t('Double editing info') + '">';
    html += '   <div>' + message + '<br/><br/>';
    html += '</div>';

    $(html).dialog({
        modal: true,
        width: 400,
        open: function() {
          $(".ui-dialog-titlebar-close > span").html(Drupal.t('Close Window') + '<img border="0" src="' + Drupal.settings.archibald.urls.module_base_path + '/images/close.png">');

          var back_button = $('<input type="button" class="form-submit" value="' + Drupal.t('back') + '"/>');

          $("#double_editing_info").append(back_button);

          $("#double_editing_info").bind( "dialogbeforeclose", function(event, ui) {
              window.history.back();
              return false;
          });

          back_button.bind('click', function() {
            window.history.back();
         });
        }
    });
}

/**
 *  display warning when user leave withou saving
 *  This will only work in modern versions of InternetExplorer and Firefox
 *  @see https://developer.mozilla.org/en/DOM/window.onbeforeunload
 */
function archibald_form_blur_message() {
    $('#archibald-content-form input').bind('change',    function () { $(window).bind('beforeunload' ,archibald_form_blur_message_unload_msg); } );
    $('#archibald-content-form textarea').bind('change', function () { $(window).bind('beforeunload' ,archibald_form_blur_message_unload_msg); } );
    $('#archibald-content-form select').bind('change',   function () { $(window).bind('beforeunload' ,archibald_form_blur_message_unload_msg); } );

    $('.archibald_form_submit_top').click( function () {  $(window).unbind('beforeunload');  } );
    $('.archibald_form_submit').click( function () {  $(window).unbind('beforeunload');  } );

    function archibald_form_blur_message_unload_msg(e) {
        e = e || window.event;

        var msg = Drupal.t('Do you really like to leave without saving?');
        if (e) {
            e.returnValue = msg;
        }
        return msg;
    };
}

})(jQuery);