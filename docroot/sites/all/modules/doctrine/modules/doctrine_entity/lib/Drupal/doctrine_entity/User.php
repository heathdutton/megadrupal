<?php

namespace Drupal\doctrine_entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * User
 *
 * @ORM\Table(name="users")
 * @ORM\Entity
 */
class User
{
  /**
   * @var integer
   *
   * @ORM\Column(name="uid", type="integer", nullable=false)
   * @ORM\Id
   * @ORM\GeneratedValue(strategy="IDENTITY")
   */
  protected $uid;

  /**
   * @var string
   *
   * @ORM\Column(name="name", type="string", length=60, nullable=false)
   */
  protected $name;

  /**
   * @var string
   *
   * @ORM\Column(name="pass", type="string", length=128, nullable=false)
   */
  protected $pass;

  /**
   * @var string
   *
   * @ORM\Column(name="mail", type="string", length=254, nullable=true)
   */
  protected $mail;

  /**
   * @var string
   *
   * @ORM\Column(name="theme", type="string", length=255, nullable=false)
   */
  protected $theme;

  /**
   * @var string
   *
   * @ORM\Column(name="signature", type="string", length=255, nullable=false)
   */
  protected $signature;

  /**
   * @var integer
   *
   * @ORM\Column(name="created", type="integer", nullable=false)
   */
  protected $created;

  /**
   * @var integer
   *
   * @ORM\Column(name="access", type="integer", nullable=false)
   */
  protected $access;

  /**
   * @var integer
   *
   * @ORM\Column(name="login", type="integer", nullable=false)
   */
  protected $login;

  /**
   * @var boolean
   *
   * @ORM\Column(name="status", type="boolean", nullable=false)
   */
  protected $status;

  /**
   * @var string
   *
   * @ORM\Column(name="timezone", type="string", length=32, nullable=true)
   */
  protected $timezone;

  /**
   * @var string
   *
   * @ORM\Column(name="language", type="string", length=12, nullable=false)
   */
  protected $language;

  /**
   * @var integer
   *
   * @ORM\Column(name="picture", type="integer", nullable=false)
   */
  protected $picture;

  /**
   * @var string
   *
   * @ORM\Column(name="init", type="string", length=254, nullable=true)
   */
  protected $init;

  /**
   * @var string
   *
   * @ORM\Column(name="data", type="blob", nullable=true)
   */
  protected $data;

  /**
   * @var \Drupal\entity\FilterFormat
   *
   * @ORM\ManyToOne(targetEntity="Drupal\entity\FilterFormat")
   * @ORM\JoinColumns({
   *   @ORM\JoinColumn(name="signature_format", referencedColumnName="format")
   * })
   */
  protected $filterFormat;


  /**
   * Get uid
   *
   * @return integer
   */
  public function getUid()
  {
    return $this->uid;
  }

  /**
   * Set name
   *
   * @param string $name
   * @return User
   */
  public function setName($name)
  {
    $this->name = $name;

    return $this;
  }

  /**
   * Get name
   *
   * @return string
   */
  public function getName()
  {
    return $this->name;
  }

  /**
   * Set pass
   *
   * @param string $pass
   * @return User
   */
  public function setPass($pass)
  {
    $this->pass = $pass;

    return $this;
  }

  /**
   * Get pass
   *
   * @return string
   */
  public function getPass()
  {
    return $this->pass;
  }

  /**
   * Set mail
   *
   * @param string $mail
   * @return User
   */
  public function setMail($mail)
  {
    $this->mail = $mail;

    return $this;
  }

  /**
   * Get mail
   *
   * @return string
   */
  public function getMail()
  {
    return $this->mail;
  }

  /**
   * Set theme
   *
   * @param string $theme
   * @return User
   */
  public function setTheme($theme)
  {
    $this->theme = $theme;

    return $this;
  }

  /**
   * Get theme
   *
   * @return string
   */
  public function getTheme()
  {
    return $this->theme;
  }

  /**
   * Set signature
   *
   * @param string $signature
   * @return User
   */
  public function setSignature($signature)
  {
    $this->signature = $signature;

    return $this;
  }

  /**
   * Get signature
   *
   * @return string
   */
  public function getSignature()
  {
    return $this->signature;
  }

  /**
   * Set created
   *
   * @param integer $created
   * @return User
   */
  public function setCreated($created)
  {
    $this->created = $created;

    return $this;
  }

  /**
   * Get created
   *
   * @return integer
   */
  public function getCreated()
  {
    return $this->created;
  }

  /**
   * Set access
   *
   * @param integer $access
   * @return User
   */
  public function setAccess($access)
  {
    $this->access = $access;

    return $this;
  }

  /**
   * Get access
   *
   * @return integer
   */
  public function getAccess()
  {
    return $this->access;
  }

  /**
   * Set login
   *
   * @param integer $login
   * @return User
   */
  public function setLogin($login)
  {
    $this->login = $login;

    return $this;
  }

  /**
   * Get login
   *
   * @return integer
   */
  public function getLogin()
  {
    return $this->login;
  }

  /**
   * Set status
   *
   * @param boolean $status
   * @return User
   */
  public function setStatus($status)
  {
    $this->status = $status;

    return $this;
  }

  /**
   * Get status
   *
   * @return boolean
   */
  public function getStatus()
  {
    return $this->status;
  }

  /**
   * Set timezone
   *
   * @param string $timezone
   * @return User
   */
  public function setTimezone($timezone)
  {
    $this->timezone = $timezone;

    return $this;
  }

  /**
   * Get timezone
   *
   * @return string
   */
  public function getTimezone()
  {
    return $this->timezone;
  }

  /**
   * Set language
   *
   * @param string $language
   * @return User
   */
  public function setLanguage($language)
  {
    $this->language = $language;

    return $this;
  }

  /**
   * Get language
   *
   * @return string
   */
  public function getLanguage()
  {
    return $this->language;
  }

  /**
   * Set picture
   *
   * @param integer $picture
   * @return User
   */
  public function setPicture($picture)
  {
    $this->picture = $picture;

    return $this;
  }

  /**
   * Get picture
   *
   * @return integer
   */
  public function getPicture()
  {
    return $this->picture;
  }

  /**
   * Set init
   *
   * @param string $init
   * @return User
   */
  public function setInit($init)
  {
    $this->init = $init;

    return $this;
  }

  /**
   * Get init
   *
   * @return string
   */
  public function getInit()
  {
    return $this->init;
  }

  /**
   * Set data
   *
   * @param string $data
   * @return User
   */
  public function setData($data)
  {
    $this->data = $data;

    return $this;
  }

  /**
   * Get data
   *
   * @return string
   */
  public function getData()
  {
    return $this->data;
  }

  /**
   * Set filterFormat
   *
   * @param \Drupal\entity\FilterFormat $filterFormat
   * @return User
   */
  public function setFilterFormat(\Drupal\entity\FilterFormat $filterFormat = null)
  {
    $this->filterFormat = $filterFormat;

    return $this;
  }

  /**
   * Get filterFormat
   *
   * @return \Drupal\entity\FilterFormat
   */
  public function getFilterFormat()
  {
    return $this->filterFormat;
  }
}
