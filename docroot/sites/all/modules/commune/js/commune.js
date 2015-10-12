// Auto upload image & hide upload button.
(function ($) {
  Drupal.behaviors.commune = {
    attach: function(context, settings) {
      $('.commune-comment-post-btn', context).hide();
      $('.commune-comment-input', context).click(function() {
          //find the related button and make visible.
          $(this).closest('form').find('.commune-comment-post-btn').show();
      });
      $('.drupal-wall-delete-btn-form', context).submit(function(e) {
         e.preventDefault();
	 if(confirm('are you sure?')) {
             return false;
         } else {
             return true;
	 }
      });

    }
  }
})(jQuery);
