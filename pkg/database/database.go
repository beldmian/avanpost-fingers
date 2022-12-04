package database

import "github.com/beldmian/finger_checker/pkg/config"

type Database struct {
	Fingerprint FingerprintsRepo
}

func Provide(c *config.Config) *Database {
	fingerprint, err := NewFingerprintsRepoPostgres(c.DBURI)
	if err != nil {
		panic("cannot initialize db")
	}
	return &Database{
		Fingerprint: fingerprint,
	}

}
