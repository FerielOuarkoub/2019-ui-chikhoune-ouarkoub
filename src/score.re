open Tennis;

type action =
  | ScorePlayerOne
  | ScorePlayerTwo
  | NewGame;

type state = {score};

let component = ReasonReact.reducerComponent("Score");

let make = _children => {
  ...component,
  initialState: () => {score: newGame},
  reducer: (action, state) =>
    switch action {
    | ScorePlayerOne =>
      ReasonReact.Update({score: score(state.score, PlayerOne)})
    | ScorePlayerTwo =>
      ReasonReact.Update({score: score(state.score, PlayerTwo)})
    | NewGame => ReasonReact.Update({score: newGame})
    },
  render: self =>
    <div>
      <h1> (ReasonReact.string("Tennis Game Counter")) </h1>
      <h2> (string_of_score(self.state.score) |> ReasonReact.string) </h2>
      (
        switch self.state.score {
        | Game(_) =>
          <button onClick=((_) => self.send(NewGame))>
            (ReasonReact.string("New Game"))
          </button>
        | _ =>
          <div>
            (ReasonReact.string("Who scored ?"))
            <button onClick=((_) => self.send(ScorePlayerOne))>
              (ReasonReact.string(" Player 1 "))
            </button>
            <button onClick=((_) => self.send(ScorePlayerTwo))>
              (ReasonReact.string(" Player 2 "))
            </button>
          </div>
        }
      )
    </div>
};