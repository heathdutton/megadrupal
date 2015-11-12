<script type="text/ng-template" id="templates/result.html">
  <h1><?php print t('Test results') ?></h1>
  
  <strong><?php print t('Your score') ?>:</strong> <span id="result-score">{{result.score}}</span>/{{test.questions.length}} (<span id="result-percentage">{{result.percentage}}%</span>)<br/>
  <strong><?php print t('Required pass percentage') ?>:</strong> {{test.pass_percentage}}%
  
  <div ng-if="wrongAnswers.length > 0">
    <h2><?php print t('Wrong Answers') ?></h2>

    <ul>
      <li ng-repeat="wrongAnswer in wrongAnswers">
        <a ng-click="open(wrongAnswer)"><?php print t('Question') ?> {{wrongAnswer + 1}}</a>
      </li>
    </ul>
  </div>

  <div ng-if="incompleteAnswers.length > 0">
    <h2><?php print t('Incomplete Answers') ?></h2>
    
    <ul>
      <li ng-repeat="incompleteAnswer in incompleteAnswers">
        <a ng-click="open(incompleteAnswer)"><?php print t('Question') ?> {{incompleteAnswer + 1}}</a>
      </li>
    </ul>
  </div>

  <a href="#/" class="button button-positive button-block"><?php print t('Take test again') ?></a>
</script>

