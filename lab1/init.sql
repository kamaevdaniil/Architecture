use stud;

CREATE TABLE `Person` (
    `login` VARCHAR(256) NOT NULL,
    `first_name` VARCHAR(256) NOT NULL,
    `last_name` VARCHAR(256) NOT NULL,
    `age` SMALLINT NOT NULL
);

INSERT INTO `Person` (`login`, `first_name`, `last_name`, `age`) VALUES
('AegriSomnia', 'Dmitry', 'Ryaboff', 23),
('Nico', 'Nicolaj', 'Andersen', 34),
('UncleCrassius', 'Crassius', 'Curio', 41),
('Liquid', 'Cameron', 'Johnson', 28);

ALTER TABLE `Person` 
    ADD PRIMARY KEY (`login`);