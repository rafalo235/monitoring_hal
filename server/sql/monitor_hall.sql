-- phpMyAdmin SQL Dump
-- version 4.0.10deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Czas wygenerowania: 19 Maj 2014, 12:15
-- Wersja serwera: 5.5.37-0ubuntu0.14.04.1
-- Wersja PHP: 5.5.9-1ubuntu4

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Baza danych: `monitor_hall`
--
CREATE DATABASE IF NOT EXISTS `monitor_hall` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `monitor_hall`;
-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `Users`
-- Uzytkownicy systemu - administratorzy
--
CREATE TABLE IF NOT EXISTS `Users` (
  `idUser` int(11) NOT NULL AUTO_INCREMENT,
  `firstName` varchar(255) NOT NULL,
  `lastName` varchar(255) NOT NULL,
  `login` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  PRIMARY KEY (`idUser`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `Companies`
-- Firmy, ktore sa wlascicielami hal. Jedna firma moze miec kilka hal.
--
CREATE TABLE IF NOT EXISTS `Companies` (
  `idCompany` int(11) NOT NULL AUTO_INCREMENT,
  `companyAddress` varchar(255) NOT NULL,
  `companyName` varchar(255) NOT NULL,
  PRIMARY KEY (`idCompany`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `Halls`
-- Dane hal. Kazda hala moze miec kilka koncentratorow
--
CREATE TABLE IF NOT EXISTS `Halls` (
  `idHall` int(11) NOT NULL AUTO_INCREMENT,
  `hallAddress` varchar(255) NOT NULL,
  `hallName` varchar(255) NOT NULL,
  `idCompany` int(11) DEFAULT NULL,
  PRIMARY KEY (`idHall`),
  FOREIGN KEY (`idCompany`) REFERENCES `Companies`(`idCompany`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `Concentrators`
-- Jedna hala moze miec kilka koncentratorow. Kazdy koncentrator kilka czujnikow
--
CREATE TABLE IF NOT EXISTS `Concentrators` (
  `idConcentrator` int(11) NOT NULL AUTO_INCREMENT,
  `idHall` int(11) DEFAULT NULL,
  PRIMARY KEY (`idConcentrator`),
  FOREIGN KEY (`idHall`) REFERENCES `Halls`(`idHall`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `Sensors`
-- Koncentrator moze miec kilka czujnikow.
--
CREATE TABLE IF NOT EXISTS `Sensors` (
  `idSensor` int(11) NOT NULL AUTO_INCREMENT,
  `idConcentratorSensor` int(11) NOT NULL, -- id czujnika dla danego koncentratora. Id czujnikow nie jest globalne.
  `idConcentrator` int(11) DEFAULT NULL,
  PRIMARY KEY (`idSensor`),
  FOREIGN KEY (`idConcentrator`) REFERENCES `Concentrators`(`idConcentrator`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `MonitorDatas`
-- Dane z czujnikow otrzymane z koncentratora - same naglowki. Dane z czujnikow sa w tabeli SensorDatas
--
CREATE TABLE IF NOT EXISTS `MonitorDatas` (
  `idMonitorData` int(11) NOT NULL AUTO_INCREMENT,
  `idPackage` bigint(20) DEFAULT NULL,
  `receiveTime` datetime NOT NULL,
  `sendTime` datetime NOT NULL,
  `sensorsAmount` int(11) DEFAULT NULL,
  `idConcentrator` int(11) DEFAULT NULL,
  PRIMARY KEY (`idMonitorData`),
  FOREIGN KEY (`idConcentrator`) REFERENCES `Concentrators`(`idConcentrator`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `SensorDatas`
-- Dane z czujnikow
--
CREATE TABLE IF NOT EXISTS `SensorDatas` (
  `idSensorData` int(11) NOT NULL AUTO_INCREMENT,
  `dangerLevel` varchar(255) NOT NULL,
  `dataStr` varchar(255) NOT NULL,
  `idData` bigint(20) NOT NULL,
  `state` varchar(255) DEFAULT NULL,
  `timeStamp` datetime NOT NULL,
  `type` varchar(255) NOT NULL,
  `idMonitorData` int(11) DEFAULT NULL,
  `idSensor` int(11) DEFAULT NULL,
  PRIMARY KEY (`idSensorData`),
  FOREIGN KEY (`idMonitorData`) REFERENCES `MonitorDatas`(`idMonitorData`),
  FOREIGN KEY (`idSensor`) REFERENCES `Sensors`(`idSensor`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `ConcentratorConfs`
-- Konfiguracja koncentratora.
--
CREATE TABLE IF NOT EXISTS `ConcentratorConfs` (
  `idConcentratorConf` int(11) NOT NULL AUTO_INCREMENT,
  `changed` bit(1) DEFAULT NULL, -- jesli 0 to czeka na wyslanie do koncentatora, 1 juz zmieniono konfiguracje
  `idPackage` bigint(20) DEFAULT NULL, -- id pakietu wysylajacego konfiguracje
  `timeStamp` datetime DEFAULT NULL,
  `idConcentrator` int(11) DEFAULT NULL,
  `idUser` int(11) DEFAULT NULL,
  PRIMARY KEY (`idConcentratorConf`),
  FOREIGN KEY (`idConcentrator`) REFERENCES `Concentrators`(`idConcentrator`),
  FOREIGN KEY (`idUser`) REFERENCES `Users`(`idUser`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `SensorConfs`
-- Konfiguracja poszczegolnych sensorow.
--
CREATE TABLE IF NOT EXISTS `SensorConfs` (
  `idSensorConf` int(11) NOT NULL AUTO_INCREMENT,
  `configType` varchar(255) NOT NULL,
  `dataStr` varchar(255) NOT NULL,
  `type` varchar(255) NOT NULL,
  `idSensor` int(11) DEFAULT NULL,
  `idConcentratorConf` int(11) DEFAULT NULL,
  PRIMARY KEY (`idSensorConf`),
  FOREIGN KEY (`idSensor`) REFERENCES `Sensors`(`idSensor`),
  FOREIGN KEY (`idSensor`) REFERENCES `ConcentratorConfs`(`idConcentratorConf`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `Requests`
-- Tabela z request konfiguracjia z koncentratora
--
CREATE TABLE IF NOT EXISTS `Requests` (
  `idRequest` int(11) NOT NULL AUTO_INCREMENT,
  `idConcentrator` int(11) DEFAULT NULL,
  `receiveTime` datetime NOT NULL,
  `idPackage` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`idRequest`),
  FOREIGN KEY (`idConcentrator`) REFERENCES `Concentrators`(`idConcentrator`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `RequestsConfs`
-- Tabela ktora zawiera informacje co koncentrator chcialby aby mu przeslac - naglowek w Requests
--
CREATE TABLE IF NOT EXISTS `RequestsConfs` (
  `idRequestConf` int(11) NOT NULL AUTO_INCREMENT,
  `configType` varchar(255) NOT NULL,
  `idRequest` int(11) DEFAULT NULL,
  PRIMARY KEY (`idRequestConf`),
  FOREIGN KEY (`idRequest`) REFERENCES `Requests`(`idRequest`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;


/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
