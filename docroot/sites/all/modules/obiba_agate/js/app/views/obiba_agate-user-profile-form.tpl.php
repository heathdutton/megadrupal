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

<tabset>
  <tab heading="<?php print t('Detail Profile'); ?>">
    <div class="row">
      <div class="col-md-6">
        <div class="obiba-bootstrap-user-register-form-wrapper">

          <div>
            <form id="obiba-user-register" name="theForm"
              ng-submit="submit(form)">
              <div sf-schema="schema" sf-form="form" sf-model="model"></div>

              <div vc-recaptcha
                theme="'light'"
                key="config.key"
                on-create="setWidgetId(widgetId)"
                on-success="setResponse(response)"></div>

              <div class="md-top-margin">
                <a tupe="button" class="btn btn-primary"
                  ng-click="onSubmit(theForm)">
                  <?php print t('Save') ?>
                </a>

                <a href="#/view" type="button" class="btn btn-default"
                  ng-click="onCancel(theForm)">
                  <?php print t('Cancel') ?>
                </a>
              </div>
            </form>
          </div>
        </div>
      </div>
    </div>
  </tab>

  <tab
    heading="<?php print t('User'); ?>"><?php print obiba_agate_get_drupal_profile_form(); ?></tab>
</tabset>
