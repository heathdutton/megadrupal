<?php

interface CloudwordsSourceControllerInterface {
  public function __construct($type);

  public function typeLabel();

  public function textGroup();

  public function textGroupLabel();

  public function targetLabel(CloudwordsTranslatable $translatable);

  public function uri(CloudwordsTranslatable $translatable);

  public function data(CloudwordsTranslatable $translatable);

  public function save(CloudwordsTranslatable $translatable);
}
