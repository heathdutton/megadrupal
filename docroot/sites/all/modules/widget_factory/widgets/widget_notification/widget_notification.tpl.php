<?php
$ng_app = drupal_html_id($preset->widget_settings['app_name']);
$ng_controller = $ng_app . 'Ctrl';
?>

<ul id="<?php echo $ng_app; ?>" ng-controller="<?php echo $ng_controller; ?>" class="widget-notification nav navbar-top-links navbar-right">
    <li class="dropdown">
        <a class="dropdown-toggle" data-toggle="dropdown" href="#">
            <span class="badge {{response.badge_type}}">{{response.badge_text}}</span>
            <i class="fa {{response.fa_icon}} fa-fw"></i>  <i class="fa fa-caret-down"></i>
        </a>
        <ul class="dropdown-menu dropdown-messages" ng-Cloak>
          <li ng-repeat="message in response.messages">
            <a ng-href="{{message.path}}">
              <p>
                <i class="fa {{message.fa_icon}} fa-fw"></i>
                <strong>{{message.title}}</strong>
                <span>{{message.subtitle}}</span>
                <span class="pull-right text-muted">
                  <em>{{message.text_muted}}</em>
                  <span class="badge {{message.badge_type}}">{{message.badge_text}}</span>
                </span>
                <div>{{message.text | limitTo:120}}{{message.text.length > 120 ? ' ...' : ''}}</div>
              </p>
              <div class="progress" ng-Show="message.showProgress">
                <div class="progress-bar {{message.progress_type}}" role="progressbar" aria-valuenow="{{message.progress_percent}}" aria-valuemin="0" aria-valuemax="100" style="width: {{message.progress_percent}}%">
                </div>
              </div>
            </a>
            <div class="divider"></div>
          </li>
          <li>
              <a class="text-center" href="{{response.detail_url}}">
                  <strong>{{response.detail_url_text}}</strong>
                  <i class="fa fa-angle-right"></i>
              </a>
          </li>
        </ul>
    </li>
</ul>

<script>
angular.module('<?php echo $ng_app; ?>', ['ngResource', 'emguo.poller'])
.controller('<?php echo $ng_controller; ?>', function($scope, $resource, poller) {
  var messageResource = $resource("<?php echo $preset->widget_settings['json_callback']; ?>");
  var messagePoller = poller.get(messageResource,{
    <?php if($preset->widget_settings['polling_delay']): ?>
    delay: <?php echo $preset->widget_settings['polling_delay']; ?>
    <?php endif; ?>
  }
  );

  messagePoller.promise.then(null, null, function (data) {
    $scope.response = data;
    $scope.hasMessages = true;
    <?php if(empty($preset->widget_settings['polling_delay'])): ?>
      messagePoller.stop();
    <?php endif; ?>
  });

});

angular.element(document).ready(function() {
  var app = document.getElementById("<?php echo $ng_app; ?>");
  angular.bootstrap(app, ['<?php echo $ng_app; ?>']);
});
</script>
