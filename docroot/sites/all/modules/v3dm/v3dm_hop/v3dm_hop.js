(function ($) {

Drupal.behaviors.hop = {
  attach: function (context, settings) {
    $('.hop').each(function() {

      var presenter = null;
      var id = $(this).attr('id');
      var file = settings.v3dm_hop.files[id];
      presenter = new Presenter(id);

      presenter.setScene({
        meshes: {
          "Mesh" : { url: file }
        },
        modelInstances : {
          "Model" : { mesh : "Mesh" }
        }
      });

    });
  }
};

}(jQuery));
