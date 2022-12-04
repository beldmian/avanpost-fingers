package config

import (
	"os"

	"gopkg.in/yaml.v3"
)

type Config struct {
	DBURI string `yaml:"db_uri"`
	Addr  string `yaml:"addr"`
}

func ParseConfig(fp string) (*Config, error) {
	var config Config
	f, err := os.ReadFile(fp)
	if err != nil {
		return nil, err
	}
	yaml.Unmarshal(f, &config)
	return &config, nil
}

func Provide() *Config {
	conf, err := ParseConfig("./config.yaml")
	if err != nil {
		panic(err)
	}
	return conf
}
