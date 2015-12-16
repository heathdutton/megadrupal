Drupal.behaviors.skaflashAdmin = {
  attach: function(context, settings) {
    setInterval(function() {
      document.getElementById('endpoint-path').innerHTML = document.getElementById('edit-skaflash-endpoint-path').value;
    }, 100);
  }
};
