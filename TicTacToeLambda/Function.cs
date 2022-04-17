using System;
using System.Linq;
using System.Text.Json;
using System.Threading.Tasks;
using Amazon;
using Amazon.GameLift;
using Amazon.GameLift.Model;
using Amazon.Lambda.APIGatewayEvents;
using Amazon.Lambda.Core;

// Assembly attribute to enable the Lambda function's JSON input to be converted into a .NET class.
[assembly: LambdaSerializer(typeof(Amazon.Lambda.Serialization.SystemTextJson.DefaultLambdaJsonSerializer))]

namespace TicTacToeLambda
{
    public class Function
    {
        private readonly AmazonGameLiftClient AwsClient;
        private readonly string FleetId;

        public Function()
        {
            var config = new AmazonGameLiftConfig();
            config.RegionEndpoint = RegionEndpoint.USEast1;

            AwsClient = new AmazonGameLiftClient(config);

            FleetId = Environment.GetEnvironmentVariable("FleetId");
        }

        /// <summary>
        /// A simple function that takes a string and does a ToUpper
        /// </summary>
        /// <param name="input"></param>
        /// <param name="context"></param>
        /// <returns></returns>
        [LambdaSerializer(typeof(Amazon.Lambda.Serialization.SystemTextJson.DefaultLambdaJsonSerializer))]
        public async Task<PlayerSession> FunctionHandler(APIGatewayProxyRequest request, ILambdaContext context)
        {
            try
            {
                var playerSession = await GetPlayerSession();

                return playerSession?.PlayerSession;
            }
            catch
            {
                return null;
            }
            //var playerSession = await GetPlayerSession();

            //if(playerSession is null)
            //{
            //    return new APIGatewayProxyResponse
            //    {
            //        StatusCode = 404
            //    };
            //}

            //return new APIGatewayProxyResponse
            //{
            //    StatusCode = 200,
            //    Body = JsonSerializer.Serialize(playerSession),
            //};
        }

        private async Task<CreatePlayerSessionResponse> GetPlayerSession()
        {
            var gameSession = await DescribeGameSesion();
            var playerId = Guid.NewGuid().ToString();

            if(gameSession is null)
            {
                var createSession = await CreateGameSession(playerId);
                gameSession = createSession.GameSession;
            }

            var playerSessionResponse = await CreatePlayerSession(gameSession, playerId, "Name");

            return playerSessionResponse;
        }


        private async Task<GameSession> DescribeGameSesion()
        {
            var request = new DescribeGameSessionsRequest()
            {
                FleetId = FleetId,
                StatusFilter = "ACTIVE"
            };

            var response = await AwsClient.DescribeGameSessionsAsync(request);

            return response.GameSessions.FirstOrDefault(x => x.CurrentPlayerSessionCount < x.MaximumPlayerSessionCount);
        }

        private async Task<CreateGameSessionResponse> CreateGameSession(string playerId)
        {
            var request = new CreateGameSessionRequest()
            {
                CreatorId = playerId,
                FleetId = FleetId,
                MaximumPlayerSessionCount = 2,
                Name = "Test Session",          // Figure out what this should be
            };

            var response = await AwsClient.CreateGameSessionAsync(request);

            return response;
        }

        private async Task<CreatePlayerSessionResponse> CreatePlayerSession(GameSession gameSession, string playerId, string playerName)
        {
            var request = new CreatePlayerSessionRequest()
            {
                GameSessionId = gameSession.GameSessionId,
                PlayerData = playerName,
                PlayerId = playerId
            };

            var response = await AwsClient.CreatePlayerSessionAsync(request);

            return response;
        }
    }
}
