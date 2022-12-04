package database

import (
	"github.com/beldmian/finger_checker/pkg/core"
	_ "github.com/jackc/pgx/stdlib"
	"github.com/jmoiron/sqlx"
)

type Fingerprint struct {
	ID       int    `db:"id"`
	Name     string `db:"name"`
	Minutias string `db:"minutias"`
}

type FingerprintsRepo interface {
	GetFingerprintByID(int) (core.Fingerprint, error)
	GetBunchFingerprints(int, int) ([]core.Fingerprint, error)
	GetFingerprintsCount() (int, error)
	InsertFingerprint(*core.Fingerprint) (bool, error)
}

type FingerprintsRepoPostgres struct {
	DB *sqlx.DB
}

func NewFingerprintsRepoPostgres(connectURI string) (*FingerprintsRepoPostgres, error) {
	db, err := sqlx.Connect("pgx", connectURI)
	if err != nil {
		return nil, err
	}
	return &FingerprintsRepoPostgres{
		DB: db,
	}, nil
}

func (repo FingerprintsRepoPostgres) GetFingerprintByID(id int) (core.Fingerprint, error) {
	fingerprint := Fingerprint{}
	if err := repo.DB.Get(&fingerprint, "SELECT * FROM fingerprints WHERE id=$1", id); err != nil {
		return core.Fingerprint{}, err
	}
	return core.Fingerprint{
		ID:       fingerprint.ID,
		Name:     fingerprint.Name,
		Minutias: core.ParseMinutias(fingerprint.Minutias),
	}, nil
}

func (repo FingerprintsRepoPostgres) GetBunchFingerprints(limit int, offset int) ([]core.Fingerprint, error) {
	fingerprints := []Fingerprint{}
	if err := repo.DB.Select(&fingerprints, "SELECT * FROM fingerprints LIMIT $1 OFFSET $2", limit, offset); err != nil {
		return nil, err
	}
	var out []core.Fingerprint
	for _, fp := range fingerprints {
		out = append(out, core.Fingerprint{
			ID:       fp.ID,
			Name:     fp.Name,
			Minutias: core.ParseMinutias(fp.Minutias),
		})
	}
	return out, nil
}

func (repo FingerprintsRepoPostgres) GetFingerprintsCount() (int, error) {
	var count int
	if err := repo.DB.Get(&count, "SELECT COUNT(*) FROM fingerprints"); err != nil {
		return -1, err
	}
	return count, nil
}

func (repo FingerprintsRepoPostgres) InsertFingerprint(fingerprint *core.Fingerprint) (bool, error) {
	tx := repo.DB.MustBegin()
	tx.MustExec("INSERT INTO fingerprints(name, minutias) VALUES ($1, $2)", fingerprint.Name, core.EncodeMinutias(fingerprint.Minutias))
	if err := tx.Commit(); err != nil {
		return false, err
	}
	return true, nil
}
