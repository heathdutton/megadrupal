<script type="text/ng-template" id="templates/test.html">
  <p ng-bind-html="test.description"></p>

  <?php print theme('table', array('rows' => array(
    array(t('Questions'), '{{test.questions.length}}'),
    array(t('Pass Percentage'), '{{test.pass_percentage}}%'),
    array(t('Time Limit'), '{{test.time_limit * 1000| date : "mm"}} min')
  )))?>
  
  <button ng-click="start()" ng-disabled="isDisabled()"><?php print t('Start Now!') ?></button>
  <p ng-if="isDisabled()"><?php print t('Your browser is not supported. Sorry.') ?></p>
</script>
