package main

import (
	"github.com/beldmian/finger_checker/pkg/config"
	"github.com/beldmian/finger_checker/pkg/database"
	"github.com/beldmian/finger_checker/pkg/logger"
	"github.com/beldmian/finger_checker/pkg/recognizer"
	"github.com/beldmian/finger_checker/pkg/server"
	"go.uber.org/fx"
	"go.uber.org/fx/fxevent"
	"go.uber.org/zap"
)

func main() {
	app := fx.New(
		fx.Provide(
			config.Provide,
			logger.Provide,
		),
		fx.WithLogger(func(log *zap.Logger) fxevent.Logger {
			return &fxevent.ZapLogger{Logger: log}
		}),
		fx.Provide(recognizer.Provide),
		fx.Provide(database.Provide),
		fx.Provide(server.Provide),
		fx.Invoke(func(*server.Server) {}),
	)
	app.Run()
}
