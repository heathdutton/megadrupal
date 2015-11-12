<?php

class VersioncontrolProjectViewsSetProjectCommitView extends VersioncontrolViewsSetGlobalBase {
  protected $setName = 'project_commit_view';

  protected $baseView = 'vc_project_commit_view';

  protected $defaultViews = array(
    'git' => 'vc_git_project_commit_view',
  );

  /**
   * This view is shown assuming a nid argument, so we can use
   * getViewNameByBackend() to define the view on a multi-backends case,
   * since one project can only have one repository(one backend) associated.
   */
  public function getViewNameByEntity(VersioncontrolEntityInterface $entity) {
    return $this->getViewNameByBackend($entity->getBackend());
  }
}
