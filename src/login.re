open User;

type action =
  | Login
  | OnChangeEmail(string)
  | OnChangePassword(string)
  | RedirectRegister;

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

type state = {user};

let component = ReasonReact.reducerComponent("Register");
let make = _children => {
  ...component,
  initialState: () => {user: initUser},
  reducer: (action, state) =>
    switch (action) {
    | Login =>
      if (state.user.email != "" && state.user.password != "") {
        postExecute(
          "http://localhost:8080/api/v1/users/login",
          encodeToJson(state.user.email, state.user.password),
        );
      } else {
        ReasonReact.SideEffects(_ => ReasonReact.Router.push("score"));
      };
      ReasonReact.SideEffects(_ => ReasonReact.Router.push("/login"));
    | OnChangeEmail(loginValue) =>
      ReasonReact.Update({user: updateEmail(state.user, loginValue)})
    | OnChangePassword(passValue) =>
      ReasonReact.Update({user: updatePassword(state.user, passValue)})
    | RedirectRegister =>
      ReasonReact.SideEffects(_ => ReasonReact.Router.push("/register"))
    },
  render: self =>
    <div>
      <h1> {ReasonReact.string(" Login ")} </h1>
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
      <label htmlFor="password"> {ReasonReact.string(" Password : ")} </label>
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
      <button onClick={_ => self.send(Login)}>
        {ReasonReact.string(" LogIn ")}
      </button>
      <br />
      <button onClick={_ => self.send(RedirectRegister)}>
        {ReasonReact.string(" Register ")}
      </button>
    </div>,
};