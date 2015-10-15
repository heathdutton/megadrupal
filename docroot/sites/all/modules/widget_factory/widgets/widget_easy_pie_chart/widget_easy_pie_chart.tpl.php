<?php
$ng_app = drupal_html_id($preset->widget_settings['app_name']);
$ng_controller = $ng_app . 'Ctrl';
?>
<div class="widget-easy-pie-chart widget-easy-pie-chart-default" ng-app="<?php echo $ng_app ?>">
  <div class="angular" ng-controller="<?php echo $ng_controller ?>">
    <div class="easypiechart clearfix" easypiechart ng-init="options = { animate:false, barColor:'#E67E22', scaleColor:false, lineWidth:3, lineCap:'butt', size: 180 }" percent="percent" options="options">
      <span class="pie-percent" ng-bind="percent"></span>
    </div>
  </div>
</div>
<script>
  var app = angular.module('<?php echo $ng_app; ?>', ['easypiechart']);
  app.controller('<?php echo $ng_controller ?>', ['$scope', function ($scope) {
    $scope.percent = <?php echo $data; ?>;
    $scope.options = {
      lineWidth: <?php echo $lineWidth; ?>,
      lineCap: '<?php echo $lineCap; ?>'
    };
  }]);

  angular.element(document).ready(function() {
    var app = document.getElementById("<?php echo $ng_app; ?>");
    angular.bootstrap(app, ['<?php echo $ng_app; ?>']);
  });
</script>
