use stud;

CREATE TABLE IF NOT EXISTS `Person` (
    `login` VARCHAR(256) NOT NULL,
    `first_name` VARCHAR(256) NOT NULL,
    `last_name` VARCHAR(256) NOT NULL,
    `age` SMALLINT NOT NULL,
    PRIMARY KEY (`login`),
    KEY `fn` (`first_name`),
    KEY `ln` (`last_name`)
);

INSERT INTO `Person` (`login`, `first_name`, `last_name`, `age`) VALUES
('AegriSomnia', 'Dmitry', 'Ryaboff', 23),
('Nico', 'Nicolaj', 'Andersen', 34),
('UncleCrassius', 'Crassius', 'Curio', 41),
('Liquid', 'Cameron', 'Johnson', 28);