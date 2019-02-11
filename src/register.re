open User;

type action =
  | Register
  | OnChangeEmail(string)
  | OnChangePassword(string)
  | OnChangeName(string)
  | RedirectLogin;

type state = {
  user,
  error: string,
};

let postExecute = (url: string, body) =>
  Js.Promise.(
    Fetch.fetchWithInit(
      url,
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=Fetch.BodyInit.make(Js.Json.stringify(body)),
        ~headers=
          Fetch.HeadersInit.make({
            "mode": "cors",
            "Content-Type": "application/json",
            "Access-Control-Allow-Origin": "*",
          }),
        (),
      ),
    )
    |> then_(value => {
         Js.log(value);
         Js.Promise.resolve(value);
       })
  );

let component = ReasonReact.reducerComponent("Register");
let make = _children => {
  ...component,
  initialState: () => {user: initUser, error: ""},
  reducer: (action, state) =>
    switch (action) {
    | Register =>
      if (state.user.email == ""
          || state.user.password == ""
          || state.user.name == ""
          ) {
        ReasonReact.Update({...state, error: "required value missing email or password or name"});
      } else {
        postExecute(
          "http://localhost:8080/api/v1/users",
          encodeToJson(state.user),
        );
        ReasonReact.SideEffects(_ => ReasonReact.Router.push("score"));
      }
    | OnChangeEmail(loginValue) =>
      ReasonReact.Update({
        ...state,
        user: updateEmail(state.user, loginValue),
      })
    | OnChangePassword(passValue) =>
      ReasonReact.Update({
        ...state,
        user: updatePassword(state.user, passValue),
      })
    | OnChangeName(nameValue) =>
      ReasonReact.Update({
        ...state,
        user: updateName(state.user, nameValue),
      })
    | RedirectLogin =>
      ReasonReact.SideEffects(_ => ReasonReact.Router.push("login"))
    },
  render: self =>
    <div>
      <h1> {ReasonReact.string(" Register ")} </h1>
      <label htmlFor="login"> {ReasonReact.string(" Email : ")} </label>
      <input
        name="login"
        id="login"
        value={self.state.user.email}
        onChange={ev => {
          let value = ReactEvent.Form.target(ev)##value;
          self.send(OnChangeEmail(value));
        }}
      />
      <br />
      <label htmlFor="pd"> {ReasonReact.string(" Password : ")} </label>
      <input
        name="password"
        id="password"
        value={self.state.user.password}
        onChange={ev => {
          let value = ReactEvent.Form.target(ev)##value;
          self.send(OnChangePassword(value));
        }}
      />
      <br />
      <label htmlFor="name">
        {ReasonReact.string(" Name : ")}
      </label>
      <input
        name="name"
        id="name"
        value={self.state.user.name}
        onChange={ev => {
          let value = ReactEvent.Form.target(ev)##value;
          self.send(OnChangeName(value));
        }}
      />
      <br />
      <div> {self.state.error |> ReasonReact.string} </div>
      <br />
      <button onClick={_ => self.send(Register)}>
        {ReasonReact.string(" OK ")}
      </button>
      <br />
      <button onClick={_ => self.send(RedirectLogin)}>
        {ReasonReact.string(" Login ")}
      </button>
    </div>,
};