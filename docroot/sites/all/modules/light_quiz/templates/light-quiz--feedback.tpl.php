<script type="text/ng-template" id="templates/feedback.html">
  <h1><?php print t('Question') ?> {{question.index + 1}}</h1>
  
  <img ng-src="data:{{question.attachment_data.content_type}};base64,{{question.attachment_data.data}}" ng-if="getType(question.attachment_data) == 'image'">

  <p ng-if="getType(question.attachment_data) == 'text'" ng-bind-html="question.attachment_data.data"></p>

  <p ng-bind-html="question.question"></p>
  
  <p ng-if="userAnswer">
    <strong><?php print t('You answer') ?>:</strong> <span ng-bind-html="userAnswer"></span>
  </p>
  
  <p ng-if="correctAnswers.length > 0">
    <strong><?php print t('Correct answer(s)') ?>:</strong>
    <ul>
      <li ng-repeat="answer in correctAnswers" ng-bind-html="answer"></li>
    </ul>
  </p>
  
  <p ng-if="question.explanation" ng-bind-html="question.explanation"></p>

  <button ng-click="back()"><?php print t('Back to results page')?></button>
</script>
