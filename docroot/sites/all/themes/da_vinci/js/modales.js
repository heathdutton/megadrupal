/**
 * @file
 * Modal output and behaviour.
 *
 * @pre Style Guide Icon setting active
 * @pre style-guide Module installed
 * @pre User admin logged in
 *
 * We get the content of the style-guide module (through ajax) into the modal window,
 * displaying a fixed icon at the right side.
 */

(function ($) {
  $(document).ready(function () {

     // @brief da_vinci_modal
     // Object Literal for setup and populating modals
     // @param id, will be the #id of your modal
     // @param url, will be the target of the ajax content
     // @param fire, will be the click element for display the modal
    var da_vinci_modal = {
      init: function(id,url,fire){
        $('body').append('<div id="' + id + '" class="generic-modal"><div class="modal-content"><a class="close" href="#">Close</a></div></div>');
        this.attach(id,url,fire);
      },
      attach: function(id,url,fire){
        p = this;
        $('#' + id + ' .modal-content').easyModal();
        $(fire).on('click',function(e){
          p.onClick(id,url,e);
        });
      },
      onClick: function(id,url,e){
        e.preventDefault();
        this.load(id,url);
        this.open(id);
      },
      load: function(id,url){
        $.get(url).then(function(data){
          $('#' + id + ' .modal-content').html($("#block-system-main",data));
          $('#' + id + ' .modal-content').css({'top':'10%','left':'10%','width':'80%','height':'80%','overflow-y':'auto','margin-left':'1px','margin-top':'1px'});
        });
      },
      open: function(id){
        $('#' + id + ' .modal-content').trigger('openModal');
      }
    };
    var style_guide_modal = da_vinci_modal.init('style-guide-modal','?q=/admin/appearance/styleguide/da_vinci','.style-guide-modal-fire');
  });
})(jQuery);
