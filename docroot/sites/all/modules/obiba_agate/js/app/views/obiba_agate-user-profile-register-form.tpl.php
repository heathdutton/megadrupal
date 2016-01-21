<?php
/**
 * @file
 * Obiba Agate Mdule.
 *
 * Copyright (c) 2015 OBiBa. All rights reserved.
 * This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

?>

<script
  src="https://www.google.com/recaptcha/api.js?onload=vcRecaptchaApiLoaded&render=explicit"
  async defer></script>

<div class="row">
  <div class="col-md-6">
    <div class="obiba-bootstrap-user-register-form-wrapper">

      <div ng-app="ObibaAgate" ng-controller="RegisterFormController">
        <alert ng-if="alert.message" type="{{alert.type}}"
          close="closeAlert($index)">{{alert.message}}
        </alert>

        <form id="obiba-user-register" name="theForm" ng-submit="submit(form)">
          <div sf-schema="schema" sf-form="form" sf-model="model"></div>

          <div vc-recaptcha
            theme="'light'"
            key="config.key"
            on-create="setWidgetId(widgetId)"
            on-success="setResponse(response)"></div>

          <div class="md-top-margin">
            <button type="submit" class="btn btn-primary"
              ng-click="onSubmit(theForm)">
              <span translate><?php print t('Join') ?></span>
            </button>

            <a href="#/" type="button" class="btn btn-default"
              ng-click="onCancel(theForm)">
              <span translate><?php print t('Cancel') ?></span>
            </a>
          </div>
        </form>
      </div>
    </div>
  </div>
</div>
