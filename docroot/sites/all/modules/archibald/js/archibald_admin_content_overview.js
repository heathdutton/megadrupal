(function ($) {

var archibald_admin_content_overview_cur_status = null;
var archibald_admin_content_overview_cur_responsible = null;
Drupal.behaviors.archibald_admin_content_overview = {
  attach: function (context, settings) {
   $('#archibald_admin_content_overview .lifecycle_status').unbind('click');
   $('#archibald_admin_content_overview .lifecycle_status').bind('click', archibald_admin_content_overview_set_lifecycle_status);
   function archibald_admin_content_overview_set_lifecycle_status() {
       var current_value = $(this).attr('status');

       archibald_admin_content_overview_cur_status = $(this);

       var archibald_modal_lifecycle_status = archibald_modal_defaults;
       archibald_modal_lifecycle_status.modalSize = {width:400, height:300, type:'absolute'};
       Drupal.CTools.Modal.show(archibald_modal_lifecycle_status);

       $.ajax({
            type: 'POST',
            url: $(this).attr('url'),
            data: {
              lom_id:$(this).attr('lom_id'),
              current_value:current_value,
              admin_content_overview_type:$('#archibald_admin_content_overview_type').val()
            },
            success: function(response, status) {
              for (var i in response) {
                if (response[i]['command'] && Drupal.ajax.prototype.commands[response[i]['command']]) {
                  Drupal.ajax.prototype.commands[response[i]['command']](Drupal.ajax, response[i], status);
                }
              }

             //   $('#modalContent span.modal-title').html(Drupal.t('Choose vCard'));
             //   $('#modalContent .modal-content').html(data);
             //   Drupal.attachBehaviors('#modalContent .modal-content');
            },
            dataType: 'json'
       });
   };

   $('#archibald_admin_content_overview .responsible_editor').unbind('click');
   $('#archibald_admin_content_overview .responsible_editor').bind('click', archibald_admin_content_overview_set_responsible_editor);
   function archibald_admin_content_overview_set_responsible_editor() {
       var current_value = jQuery.trim( $(this).text() );

       archibald_admin_content_overview_cur_responsible = $(this);
       Drupal.CTools.Modal.show();

       $.ajax({
            type: 'POST',
            url: $(this).attr('url'),
            data: {
              lom_id:$(this).attr('lom_id'),
              admin_content_overview_type:$('#archibald_admin_content_overview_type').val()
            },
            success: function(response, status) {
              for (var i in response) {
                if (response[i]['command'] && Drupal.ajax.prototype.commands[response[i]['command']]) {
                  Drupal.ajax.prototype.commands[response[i]['command']](Drupal.ajax, response[i], status);
                }
              }
            },
            dataType: 'json'
       });
   };

   $('#archibald_admin_content_overview .content_partner').unbind('click');
   $('#archibald_admin_content_overview .content_partner').bind('click', archibald_admin_content_overview_set_content_partner);
   function archibald_admin_content_overview_set_content_partner() {
       var archibald_modal_content_partner = archibald_modal_defaults;
       archibald_modal_content_partner.modalSize = {width:400, height:300, type:'absolute'};
       Drupal.CTools.Modal.show(archibald_modal_content_partner);

       $.ajax({
            type: 'POST',
            url: $(this).attr('url'),
            data: {
              admin_content_overview_type:$('#archibald_admin_content_overview_type').val()
            },
            success: function(response, status) {
              for (var i in response) {
                if (response[i]['command'] && Drupal.ajax.prototype.commands[response[i]['command']]) {
                  Drupal.ajax.prototype.commands[response[i]['command']](Drupal.ajax, response[i], status);
                }
              }
            },
            dataType: 'json'
       });
   };

   // handler for the set new lifecycle.status popup mail options
   $('.new_life_cycle_status_selector').unbind('change');
   $('.new_life_cycle_status_selector').bind('change', archibald_admin_content_overview_life_cycle_status);
   function archibald_admin_content_overview_life_cycle_status() {
       var old_status = $('.new_life_cycle_status_old_status').val();
       var new_status = $(this).val();
       var subject = $('.new_life_cycle_status_mail_subject').parents('.form-item-mailsubject');
       var body = $('.new_life_cycle_status_mail_body').parents('.form-item-mailtext');
       var mail_block = $('.new_life_cycle_status_mail_block');

       if (new_status.indexOf('|')!=-1) {
        new_status = new_status.substr(new_status.indexOf('|')+1);
       }

       if (old_status!='final' && new_status=='final') {
           mail_block.show();
           subject.hide();
           body.hide();

       } else if (old_status=='final' && new_status!='final') {
           mail_block.show();
           subject.show();
           body.show();

           var width = 1000;
           var height = 500;

           // Use the additionol pixels for creating the width and height.
           $('div.ctools-modal-content').css({
            'width': width + Drupal.CTools.Modal.currentSettings.modalSize.addWidth + 'px',
            'height': height + Drupal.CTools.Modal.currentSettings.modalSize.addHeight + 'px'
           });
           $('div.ctools-modal-content .modal-content').css({
            'width': (width - Drupal.CTools.Modal.currentSettings.modalSize.contentRight) + 'px',
            'height': (height - Drupal.CTools.Modal.currentSettings.modalSize.contentBottom) + 'px'
           });
           $(window).trigger('resize'); // lets cTools recalculate left and top values

       } else {
           mail_block.hide();
       }
   };


   $('#archibald_admin_content_overview .publish_link').unbind('click');
   $('#archibald_admin_content_overview .publish_link').bind('click', archibald_admin_content_overview_publish);
   function archibald_admin_content_overview_publish(event) {
       event.preventDefault();

       var archibald_modal_publish = archibald_modal_defaults;
       archibald_modal_publish.modalSize = {width:500, height:500, type:'absolute'};
       Drupal.CTools.Modal.show(archibald_modal_publish);

       $.ajax({
            type: 'POST',
            url: $(this).attr('href'),
            data: {
              ajax:true,
              admin_content_overview_type:$('#archibald_admin_content_overview_type').val()
            },
            success: function(response, status) {
              for (var i in response) {
                if (response[i]['command'] && Drupal.ajax.prototype.commands[response[i]['command']]) {
                  Drupal.ajax.prototype.commands[response[i]['command']](Drupal.ajax, response[i], status);
                }
              }
            },
            dataType: 'json'
       });
   };
  }
};

})(jQuery);
