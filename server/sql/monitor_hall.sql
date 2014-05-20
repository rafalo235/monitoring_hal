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
--
CREATE TABLE IF NOT EXISTS `Companies` (
  `idCompany` int(11) NOT NULL AUTO_INCREMENT,
  `companyAddress` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`idCompany`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `Halls`
--
CREATE TABLE IF NOT EXISTS `Halls` (
  `idHall` int(11) NOT NULL AUTO_INCREMENT,
  `address` varchar(255) NOT NULL,
  `hallName` varchar(255) NOT NULL,
  `idCompanay` int(11) DEFAULT NULL,
  PRIMARY KEY (`idHall`),
  FOREIGN KEY (`idCompanay`) REFERENCES `Companies`(`idCompany`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `Concentrators`
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
--
CREATE TABLE IF NOT EXISTS `Sensors` (
  `idSensor` int(11) NOT NULL AUTO_INCREMENT,
  `idConcentratorSensor` int(11) NOT NULL,
  `idConcentrator` int(11) DEFAULT NULL,
  PRIMARY KEY (`idSensor`),
  FOREIGN KEY (`idConcentrator`) REFERENCES `Concentrators`(`idConcentrator`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `MonitorDatas`
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
--
CREATE TABLE IF NOT EXISTS `ConcentratorConfs` (
  `idConcentratorConf` int(11) NOT NULL AUTO_INCREMENT,
  `changed` bit(1) DEFAULT NULL,
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
--
CREATE TABLE IF NOT EXISTS `SensorConfs` (
  `idSensorConf` int(11) NOT NULL AUTO_INCREMENT,
  `configType` varchar(255) NOT NULL,
  `dataStr` varchar(255) NOT NULL,
  `type` varchar(255) NOT NULL,
  `idSensor` int(11) DEFAULT NULL,
  PRIMARY KEY (`idSensorConf`),
  FOREIGN KEY (`idSensor`) REFERENCES `Sensors`(`idSensor`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `Requests`
--
CREATE TABLE IF NOT EXISTS `Requests` (
  `idRequest` int(11) NOT NULL AUTO_INCREMENT,
  `idConcentrator` int(11) DEFAULT NULL,
  PRIMARY KEY (`idRequest`),
  FOREIGN KEY (`idConcentrator`) REFERENCES `Concentrators`(`idConcentrator`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------
--
-- Struktura tabeli dla tabeli `RequestsConfs`
--
CREATE TABLE IF NOT EXISTS `RequestsConf` (
  `idRequestConf` int(11) NOT NULL AUTO_INCREMENT,
  `configType` varchar(255) NOT NULL,
  `idRequest` int(11) DEFAULT NULL,
  PRIMARY KEY (`idRequestConf`),
  FOREIGN KEY (`idRequest`) REFERENCES `Requests`(`idRequest`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;


/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
