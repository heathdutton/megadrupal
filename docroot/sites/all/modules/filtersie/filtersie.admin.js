(function ($) {

  Drupal.behaviors.filtersieModule = {
    attach: function (context, settings) {
      var This = this;
      $('.filtersie-matrix-wrapper').each(function() {
        var $matrix_wrapper = $(this);
        var $matrixInputs = $matrix_wrapper.find('input');
        This.sumEntrie($matrixInputs, $matrix_wrapper);
        $matrixInputs.change(function() {
          This.sumEntrie($matrixInputs, $matrix_wrapper);
        });
      });
    },
    sumEntrie: function(entries, context) {
      var out = 0;
      $.each(entries, function(index,entry){
        var f = parseFloat($(entry).val());
        out+=f?f:0;
      });
      $('.filtersie-matrix-sum', context).html(out);
      return out;
    }
  };

})(jQuery);
