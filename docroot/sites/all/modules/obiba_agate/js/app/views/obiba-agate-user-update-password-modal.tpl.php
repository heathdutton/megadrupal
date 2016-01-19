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

<div class="modal-header">
  <h3 class="modal-title"><?php print t('Update the password'); ?></h3>
</div>
<obiba-alert id="ModalPasswordUpdateController"></obiba-alert>
<div class="container">
  <form name="updatePassword">
    <label class="control-label "
      for="password"><?php print t('The current password'); ?></label>
    <input ng-model="profile.password" class="form-control" type="password"/>

    <label class="control-label "
      for="NewPassword"><?php print t('New password'); ?></label>
    <input ng-model="profile.NewPassword" class="form-control" type="password"/>
    <password-strength-bar
      password-to-check="profile.NewPassword"></password-strength-bar>
    <div
      ng-class="(profile.ConfirmPassword)?((profile.ConfirmPassword==profile.NewPassword)?'has-success':'has-error'):''">
      <label class="control-label "
        for="ConfirmPassword"><?php print t('Password confirmation'); ?></label>
      <input ng-model="profile.ConfirmPassword" class="form-control "
        type="password"/>
    </div>
    <input ng-model="UserId" class="form-control" type="hidden"/>
    {{UserId}}
  </form>
</div>
<div class="modal-footer">
  <button class="btn btn-primary" ng-click="ok()">OK</button>
  <button class="btn btn-default" ng-click="cancel()">Cancel</button>
</div>
