jQuery('document').ready(function() {
  var $last = { top:0, left:0 };
  jQuery('.view-views-media-browser .views-row').each(
    function() {
      var $this = jQuery(this);
      var $offset = $this.offset();
      if($offset.top!=$last.top) {
        $this.before('<div class="clearfix"></div>');
      };
      $offset = $this.offset();
      $last=$offset;
    }
  )
});
