(function($) {
  Drupal.behaviors.custom_pub_admin = {
    attach : function() {
      $("td.custom_pub-option-edit-cell").html('<a href="#">Edit</a>').css("text-align", "right");
      $("tr.custom_pub-form-edit").hide();
      $("td.custom_pub-option-edit-cell > a").bind('click', function() {
        $this = $(this);
        $opt = new Array();
        $opt["Edit"] = "Close";
        $opt["Close"] = "Edit";
        $this.parents('tr.custom_pub-option-row').next("tr.custom_pub-form-edit").toggle();
        $txt = $this.text();
        $this.text($opt[$txt]);
        return false;
      });
      $("th.close-cusotm-pub-table").html('<a>X</a>')
      .css('text-align', 'right');
      $("th.close-cusotm-pub-table > a")
      .css('cursor', 'pointer')
      .live('click', function() {
        $this = $(this);
        $opt = new Array();
        $opt["Edit"] = "Close";
        $opt["Close"] = "Edit";
        $row = $this.parents("tr.custom_pub-form-edit");
        $row.toggle()
        $link = $row.prev('tr.custom_pub-option-row').find("td.custom_pub-option-edit-cell > a");
        $txt = $link.text();
        $link.text($opt[$txt]);
        return false;
      });
    }
  }
})(jQuery);
